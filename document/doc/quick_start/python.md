# Game of Life in Python

## Install

Installing the LUE Conda package is the easiest way to get started:

```bash
conda install -c conda-forge lue
```

See also [](#install-conda).


## Implement

Building models using the LUE Python package for map algebra is similar to using the popular Numpy Python
package for array algebra. In both packages values are organized in arrays which can be translated using
function and operator syntax. Many of the functions are overloaded for scalar values. The next example can be
executed using both LUE and Numpy, and even with pure Python if only scalars are used:

```python
# b and c are arrays or scalars
a = b + c
d = sqrt(a + 5.0)
```

When using LUE, the *definition* of the work that must be performed is separated from the *execution* of this
work. LUE work is created asynchronously. This enables LUE to solve the puzzle of using all available hardware
efficiently. In principle, some of the work created by the `sqrt` function in the previous example can be
scheduled at the same time as some of the work created by the `+` operators. As long as the results are
correct.

For the model developer this asynchrony has a few practical implications, related to:

- Managing the runtime
- Dealing with asynchronously computed values
- Slowing down the interpreter


### Managing the runtime

The runtime is the software that manages all the asynchronously defined work that must be performed. Its
responsibility includes the scheduling of the work on workers (CPU cores) as efficiently as possible. Before
executing LUE operations, the runtime must be started, after which it runs invisible to the user until all
work has been performed. LUE includes a function decorator, called `runtime_scope` that starts the runtime
automatically. In the majority of the cases code similar to the following can be used to manage the runtime:

```python
import lue.framework as lfr

# LUE operations cannot be used here


@lfr.runtime_scope
def run_model():
    # LUE operations can be used here and in all code called from here
    # The function blocks at the end, until all LUE work has been performed
    # ...


# LUE operations cannot be used here

run_model()

# LUE operations cannot be used here
```

The name of the decorated function (here `run_model`) is not relevant.

```{note}
When multiple processes are used to execute a LUE model, then the function decorated with `runtime_scope` is
only executed by one of the processes[^decorator]. Unless special measures are taken, code before and after
the call to the decorated function is executed by all processes.
```

[^decorator]: The `runtime_scope` decorator itself is executed by all processes. This is needed because the
runtime needs to be started by all processes, allowing them to collaboratively solve all the work.


### Dealing with asynchronously computed values

All LUE operations return asynchronously computed values, which may or may not be ready to be used. In
general, asynchronously computed values are not ready to be used. They can be forwarded, as arguments, to
other LUE operations though. As long as LUE operations are used, the fact that values are asynchronously
computed is not relevant to the model developer. This is not the case when values must be forwarded to non-LUE
operations. Then, the value must be waited upon. This blocks the Python interpreter and thus the creation of
tasks by LUE operations in subsequent expressions.

```python
lowest_elevation = lfr.minimum(elevation)

# Using the value means explicitly get-ting it. This blocks the interpreter.
print(lowest_elevation.get())

# We only get here once the computation of lowest_elevation has finished
```

```{note}
Use LUE operations whenever possible. Mixing LUE operations with other library's operations requires blocking
the interpreter, which limits the options of the LUE runtime for efficiently performing all the work.
```


### Slowing down the interpreter

Calling many LUE operations in a loop, like when simulating processes through time, can result in a large
amount of work being forwarded to the runtime for execution. All this work uses memory and adds to the
overheads of the runtime managing it. Work created for future time steps often depends on earlier time steps
being finished. As long as enough work is available to keep all workers busy, there is no advantage to create
more work. LUE has a facility to limit the number of iterations for which work is created. This is called the
`rate_limit` and its value informs LUE to not create work for more than this number of iterations. Whenever
the work for an iteration is finished, work for another iteration is added to the mix. This way, LUE ensures
that there is always work for `rate_limit` iterations is available for execution. The next example illustrates
the use of `rate_limit`.


```python
class MyModel(lfr.Model):

    def __init__(self):
        super().__init__()

    def initialize(self, time_step):
        # Initialize the system's state
        # ...

    def simulate(self, time_step):
        # Simulate the system's state for one time step
        # ...


        # Inform LUE about when the time step is finished. A partitioned array
        # can be asked for a future which becomes ready once all partitions have
        # become ready.
        return state.future()


my_model = MyModel();

# Supply the runtime with work for 3 time steps
lfr.run_deterministic(my_model, lfr.DefaultProgressor(), nr_time_steps=365 * 24, rate_limit=3);
```


### Game of Life

For the implementation of the Game of Life model, the following aspects can be identified that need to be
supported:

- Command line argument parsing. It is convenient if some aspects of the model, like the shape of the array
  and the number of iterations to simulate, can be customized when starting the model.
- Initialize a generation. A generation is a distribution of alive and dead cells.
- Determine a next generation based on a current generation
- Save generations to files

We will implement these aspects in turn. See [](#quick-start-model) for the complete script.


#### Parse the command line arguments

Many libraries exist that help with parsing command line arguments. A convenient one is called
[docopt](https://docopt.org), which can be installed using Pip and Conda. When using LUE, how you handle
argument parsing, if at all, is up to you, of course. In our example, argument parsing is done like this
(simplified):

```python
def parse_command_line() -> dict:
    usage = """\
Usage:
    {command} [--partition_shape=<shape>] <array_shape> <nr_generations> <pathname>
""".format(command=Path(sys.argv[0]).name)

    arguments = docopt.docopt(usage, sys.argv)
    array_shape = parse_shape(arguments["<array_shape>"])
    nr_generations = int(arguments["<nr_generations>"])
    partition_shape = arguments["--partition_shape"]

    if partition_shape is not None:
        partition_shape = parse_shape(partition_shape)

    path = Path(arguments["<pathname>"])

    return {
        "array_shape": array_shape,
        "partition_shape": partition_shape,
        "nr_generations": nr_generations,
        "generation_path": path,
    }
```

Handling command line argument parsing with docopt is a matter of defining the usage string, asking docopt to
parse the command line arguments given the usage string, and querying the dictionary returned by docopt.

Here, we require that the user passes in an array shape (formatted as `[<nr_rows>,<nr_cols>]`, e.g.:
`[2000,2000]`), the number of generations to simulated (a non-negative integer), and the pathname, without
extension, of the rasters to write the generations in. Per raster a generation number will be added to the
final pathname. Optionally, a partition shape can be passed in as well, which will be used to partition the
array. By default LUE will use a heuristic to determine a suitable partition shape, based on the array size
and the number of CPU cores used.


#### Initialize a generation

A generation in Game of Life is an array with boolean values representing whether or not a cell is alive or
dead. To initialize this array, we generate uniformly distributed random values between 0.0 and 1.0, and test
for each of them whether they are less or equal than some fraction (here 0.25). The result is an array where
~25% of the cells are alive.

```python
def initialize_generation(array_shape: Shape, partition_shape: Shape | None) -> Generation:
    fraction_alive_cells = 0.25
    random_field = lfr.uniform(
        array_shape,
        np.dtype(np.float32),
        min_value=0,
        max_value=1,
        partition_shape=partition_shape,
    )
    generation = random_field <= fraction_alive_cells

    return generation
```


#### Simulate the next generation

Finally, the core function of the Game of Life model: a function that simulates a next generation based on a
current generation. Here we have to encode the cellular automata rules which are characteristic for the Game
of Life model. The result is a new distritution of alive and dead cells.

```python
def next_generation(generation: Generation) -> Generation:
    kernel = np.array(
        [
            [1, 1, 1],
            [1, 0, 1],
            [1, 1, 1],
        ],
        dtype=np.uint8,
    )
    nr_alive_cells = lfr.focal_sum(generation, kernel)

    # Next state of currently alive cells
    underpopulated = nr_alive_cells < 2
    overpopulated = nr_alive_cells > 3

    # Next state of currently dead cells
    reproducing = nr_alive_cells == 3

    generation = lfr.where(
        generation,
        # True if alive and not under/overpopulated
        ~(underpopulated | overpopulated),
        # True if dead with three neighbours
        reproducing,
    )

    return generation
```

In this function different kinds of LUE operations are used:

- A focal operation: `focal_sum`
- Local operations:
    - Conditional: `where`
    - Comparison: `less_then` / `<`, `greater_then` / `>`, and `equal_to` / `==`
    - Logical: `not` / `~`, `or` / `|`[^logical_not]

[^logical_not]: LUE forbids arrays to be used in [truth value
testing](https://docs.python.org/3/library/stdtypes.html#truth-value-testing) because this is ambiguous: how
to convert an array of values to a single True or False? To allow for Boolean operations on a cell by cell
basis, LUE uses the [Python bitwise
operators](https://docs.python.org/3/library/stdtypes.html#bitwise-operations-on-integer-types): `&` for
Boolean `and`, `|` for Boolean `or`, `^` for Boolean exclusive or, and `~` for Boolean `not`.


#### Save a generation

LUE provides two functions to perform I/O to raster formats supported by the [GDAL](https://www.gdal.org)
library. With `from_gdal` a raster in any supported format can be read, and with `to_gdal` a raster in GeoTIFF
format can be written.

```python
lfr.to_gdal(generation, f"{self.generation_path}-{generation_id}.tif")
```

This function also works asynchronously, so Python will continue interpreting subsequent statements while the
I/O is being scheduled for execution, at some future moment in time.


#### Putting it all together

We now have the building blocks for finishing the script that implements a version of the Game of Life
cellular automata with LUE. LUE contains a base class for specialized model classes. We can use that here to
implement a `GameOfLife` model class, which we can then pass to LUE's `run_deterministic` function, which will
run the model for us, optionally reporting progress and limiting the rate with which work is created.
Specializing LUE's `Model` class and using `run_deterministic` is not required, though. We could also use a
set of free functions and a custom iteration.

A LUE model simulating a system's state through time must implement a `simulate` member function to simulate
the change of the state for a single time step, and optionally an `initialize` member function to initialize
the systems's state. In our case, this is where we can call our `initialize_generation` and `next_generation`
functions. At the end of the `simulate` we save the new distribution of alive and dead cells.


```python
class GameOfLife(lfr.Model):

    def __init__(self, array_shape: Shape, partition_shape: Shape | None, generation_path: Path):
        super().__init__()
        self.array_shape = array_shape
        self.partition_shape = partition_shape
        self.generation_path = generation_path

    def save_generation(self,
        generation: Generation,
        generation_id: int) -> None:

        lfr.to_gdal(generation, f"{self.generation_path}-{generation_id}.tif")

    def initialize(self):
        self.generation = initialize_generation(array_shape=self.array_shape, partition_shape=self.partition_shape)

    def simulate(self, iteration: int):
        self.generation = next_generation(self.generation)
        self.save_generation(self.generation, iteration)

        return self.generation.future()
```

To actually run the model we call `run_deterministic`. This function takes a `Model` specialization, a
progressor for reporting progress, the number of time steps to simulate and optionally a rate limit. Since
this is the point in the script where we start calling code that uses the LUE framework API, we must use the
`runtime_scope` decorator.

```python
@lfr.runtime_scope
def game_of_life(
    *,
    array_shape: Shape,
    partition_shape: Shape | None,
    nr_generations: int,
    generation_path: Path,
) -> None:

    model = GameOfLife(array_shape=array_shape, partition_shape=partition_shape, generation_path=generation_path)

    lfr.run_deterministic(model, lfr.DefaultProgressor(), nr_time_steps=nr_generations, rate_limit=4)
```

What remains is a `main` function which must be called when the script is executed (instead of imported). It
initiates the argument parsing and forwards the parsed arguments to the function that will start our model.

```python
def main():
    arguments = parse_command_line()
    game_of_life(**arguments)


if __name__ == "__main__":
    main()
```


## Execute

We can execute our model as we would execute any Python script:

```bash
# Array shape may need to be quoted, depending on the shell used
python game_of_life.py [2000,2000] 20 generation
```

This will start a single process using all (real) CPU cores available in the computer. It is possible to
configure the runtime used to execute all the work. For example, it is possible to limit the number of CPU
cores that must be used. How to do this is documented in the HPX documentation page about [HPX command line
options](https://hpx-docs.stellar-group.org/latest/html/manual/launching_and_configuring_hpx_applications.html#hpx-command-line-options)[^HPX].
All HPX-specific command line arguments are passed on to the runtime. To execute our model on just two CPU
cores, we can use the `--hpx:threads` option:

[^HPX]: HPX is the library that implements the runtime used by LUE.

```bash
# Array shape may need to be quoted, depending on the shell used
python game_of_life.py --hpx:threads=2 [2000,2000] 20 generation
```

More information about executing LUE models can be found on the [](execute-lue-framework-programs) page.


(quick-start-model)=

## Model

```{literalinclude} game_of_life.py
   :caption: game_of_life.py
```
