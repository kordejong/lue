(doc-reference-operation)=

# Operations

Operations are the generic building blocks for larger applications. Most operations use some computation to
combine and translate one or more arguments into one or more results. For example, the
{py:func}`convolve` operation combines an array of floating point values with a kernel of
floating point weights and computes a result array with floating point values.

```python
array = convolve(array, kernel)
```

Operations can be called using syntax that depends on the programming language used. Function call syntax is
always supported and some combinations of a programming language and an operation support operator syntax as
well.

```python
# Function syntax
result1, result2 = some_function(argument1, argument2)

# Operator syntax
result = argument1 some_binary_operator argument2
```

Operations can be combined to define large computations, like simulation models.

In LUE, operations asynchronously compute results, using a collection of interdependent asynchronous tasks.
Tasks execute as soon as all (asynchronously computed!) arguments are ready to be used and hardware is
available to them. As operations only create tasks and don't wait for them to finish executing, they
typically return much sooner than their tasks finish executing. This is a good thing. That way, tasks created
by multiple operations are available for the scheduler to select for execution, keeping all hardware busy
doing useful work.

```{note}
- Not all available operations are documented already. In Python the `help` function can be used to obtain
  more information (e.g.: `help(lue.framework.uniform)`).
- Except when explicitly stated, all operations require the input arguments to be "compatible". In the case
  of rasters, this implies that each raster must cover the same area, have the same resolution, and have the
  same coordinate reference system. LUE won't implicitly convert arguments to become compatible. Operations
  exist that help with such conversion, like {py:func}`resample`, for example.
- The signatures of the operations are written in a language-agnostic manner. How to use each operation from
  a specific language might slightly differ. Check the usage examples for inspiration.
- The most relevant part of each usage example is the call of the operation. Some of the code surrounding
  such calls is atypical code developed specifically to make it easy to write usage examples. This code, from
  the `lue_document` C++ library and the `lue.document` Python subpackage, should not be used in real-world
  LUE code.
```

## Local operations

<img src="figure/local_operation.*" width="250" align="right"/>

Local operations (AKA point operations) compute values for each cell in the result array, given one or more
corresponding cells in one or more argument arrays.

Local operations differ with respect to how they compute a result value for a
cell, given one or more argument values.

An example of a local operation is the {py:func}`add`
operation, which assigns the sum of argument cell values to the corresponding result cell.

See [](reference-operation-local) for an overview of all local operations.

## Focal operations

<img src="figure/focal_operation.*" width="250" align="right"/>

Focal operations (AKA filter operations, neighbourhood operations, window operations) compute values for each
cell in the result array, given values within a kernel centered on each corresponding (focal) cell in the
argument array. Kernels have a shape and contain weights with which to multiply each corresponding cell in the
argument array.

Focal operations differ with respect to how they compute a result value given argument values.

An example of a focal operation is the {py:func}`focal_maximum` operation, which assigns to each
focal result element the maximum value in the values found in the neigbourhood around the corresponding focal
argument cell.
See [](reference-operation-focal) for an overview of all focal operations.

## Zonal operations

<img src="figure/zonal_operation.*" width="250" align="right"/>

Zonal operations (AKA area operations) compute values for each cell in the result array, given corresponding
(zonal) cells in an argument array. Each cell in the argument array belongs to a class, each of which defines
a zone. Result values are computed per zone, so cells in the result array located within the same zone receive
the same value.

Zonal operations differ with respect to how they compute a result given the argument values per zone.

An example of a zonal operation is the {py:func}`zonal_sum` operation, which assigns to each
zonal result element the sum of the values found in the zone of the corresponding zonal argument cell.
See [](reference-operation-zonal) for an overview of all zonal operations.

## Global operations

Global operations (AKA map operations) compute a single result value, given all cell values in the input argument array.

Global operations differ with respect to how they compute a result given all values.

An example of a global operation is the {py:func}`mimimum` operation, which returns the minimum
value of all element values in the argument.

See [](reference-operation-global) for an overview of all global operations.

## Routing operations

<img src="figure/routing_operation.*" width="250" align="right"/>

In general, routing operations compute result element values, given argument element values, while taking a
topological network into account. Only argument values from elements that are somehow connected are taken into
account when computing a result element value. Networks can be defined in multiple ways. A common approach is
to use a flow direction field represented by an array, in which each cell contains a code representing a
direction to which it drains.

Other operations involving networks, for example, those creating a network as a result instead of using one as
an argument, are also grouped as routing operations.

Routing operations differ with respect to how they compute a result given the argument values reachable using
a network, or how this network is represented.

An example of routing operation is the {py:func}`d8_flow_direction` operation, which returns a so-called D8
flow direction field, given an elevation model as argument. Routing operations can, for example, be used to
simulate the flow of surface water through a landscape.

See [](reference-operation-routing) for an overview of all routing operations.

## I/O operations

TODO: description

See [](reference-operation-io) for an overview of all I/O operations.

## Conversion operations

TODO: description

See [](reference-operation-conversion) for an overview of all conversion operations.

## Miscellaneous operations

TODO: description

See [](reference-operation-miscellaneous) for an overview of all miscellaneous operations.

<!--
iterate_per_element

arithmetic_element_types
boolean_element_type
count_element_type
floating_point_element_types
flow_direction_element_type
id_element_type
index_element_type
integral_element_types
material_element_types
signed_arithmetic_element_types
signed_integral_element_types
unsigned_integral_element_types
zone_element_types

Connectivity

DownscaleStrategy

Hyperslab

PartitionedArray<float32, 2>
PartitionedArray<float64, 2>
PartitionedArray<int32, 2>
PartitionedArray<int64, 2>
PartitionedArray<int8, 2>
PartitionedArray<uint32, 2>
PartitionedArray<uint64, 2>
PartitionedArray<uint8, 2>

Scalar<float32>
Scalar<float64>
Scalar<int32>
Scalar<int64>
Scalar<int8>
Scalar<uint32>
Scalar<uint64>
Scalar<uint8>

SerialRoute_int32_2
SerialRoute_int64_2
SerialRoute_int8_2
SerialRoute_uint32_2
SerialRoute_uint64_2
SerialRoute_uint8_2

DefaultProgressor
DynamicModel
DynamicModelRunner
Model
MonteCarloModel
MonteCarloModelRunner
Progressor
SilentProgressor
StaticModel
StaticModelRunner
run_deterministic
run_stochastic

runtime_scope
default_hpx_runtime_configuration
on_root_locality
start_hpx_runtime
stop_hpx_runtime
with_development_operations
support

future<void>

lfr

shared_future<float32>
shared_future<float64>
shared_future<int32>
shared_future<int64>
shared_future<int8>
shared_future<std::map<std::int32_t, std::vector<double>>>
shared_future<std::map<std::int32_t, std::vector<float>>>
shared_future<std::map<std::int64_t, std::vector<double>>>
shared_future<std::map<std::int64_t, std::vector<float>>>
shared_future<std::map<std::int8_t, std::vector<double>>>
shared_future<std::map<std::int8_t, std::vector<float>>>
shared_future<std::map<std::uint32_t, std::vector<double>>>
shared_future<std::map<std::uint32_t, std::vector<float>>>
shared_future<std::map<std::uint64_t, std::vector<double>>>
shared_future<std::map<std::uint64_t, std::vector<float>>>
shared_future<std::map<std::uint8_t, std::vector<double>>>
shared_future<std::map<std::uint8_t, std::vector<float>>>
shared_future<std::vector<lue::PartitionedArray<double, 2>>>
shared_future<std::vector<lue::PartitionedArray<float, 2>>>
shared_future<std::vector<lue::PartitionedArray<std::int32_t, 2>>>
shared_future<std::vector<lue::PartitionedArray<std::int64_t, 2>>>
shared_future<std::vector<lue::PartitionedArray<std::int8_t, 2>>>
shared_future<std::vector<lue::PartitionedArray<std::uint32_t, 2>>>
shared_future<std::vector<lue::PartitionedArray<std::uint64_t, 2>>>
shared_future<std::vector<lue::PartitionedArray<std::uint8_t, 2>>>
shared_future<uint32>
shared_future<uint64>
shared_future<uint8>
shared_future<void>
-->
