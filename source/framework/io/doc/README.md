# I/O

## LUE data model

Terminology:

- A LUE dataset is represented by an HDF5 file
- A LUE array is represented by an HDF5 dataset

### Notes

- Reading or writing to a dataset must happen after any previous calls to reading and writing to that same
dataset have finished. See usage of `to_lue_order` and `from_lue_order`. In theory, multiple reads to the same
file can start concurrently, but in the parallel I/O case this messes up the handling of collective calls,
which need to happen in the same order in all processes. This can be made to work, but at the cost of quite
some complexity.
- No two processes can have the same dataset open for writing
- Thread-safety and HDF5:
  - Threadsafe:
    - Reading and writing can be done concurrently. Any serialization happens in the HDF5 library. This may
    cause concurrent HPX tasks to block in the HDF5 library, though(?). It may be better to serialize on the
    LUE side. Perform I/O from a single HPX task.
  - Non-threadsafe:
    - Writing to an array must be done serialized, from the same OS thread used to open and close the file.
    All HDF5 API calls must be done from the same OS thread. It is required to serialize all calls on the LUE
    side. Perform I/O from a single HPX task.
- Parallel I/O:
  - Collective operations, like opening and closing an HDF5 file, need to happen in the same order in all
  processes
  - Non-collective operations, like reading and writing from/to an HDF5 dataset can be done independently

### Read

- Reading from multiple processes at the same time is fine

#### Root process

- Group partitions by process
- Asynchronously wait on any previous `to_lue` and `from_lue` calls
- Spawn a task per process to read into its partitions

#### Worker process

- Asynchronously wait for all partitions to become ready (computations writing into them finished)
- Open dataset for reading
- Synchronously read hyperslabs corresponding to the worker's partitions from the HDF5 dataset into the LUE
array. In case of parallel I/O, configure the HDF5 transfer mode as `HDF5D_MPIO_INDEPENDENT`.

### Write

Serial I/O:

- It is not possible for multiple processes to write to the same dataset at the same time

Parallel I/O:

- Writing from multiple processes at the same time is fine

#### Root process

- Group partitions by process
- Asynchronously wait on any previous `to_lue` and `from_lue` calls
- Spawn a task per process to write from its partitions. Only spawn a new task once the previous write
    has finished (the dataset is closed again).

Notes:

- Currently, we iterate over the localities in order. We could take the readiness of the partitions located in
  each locality into account, sending write tasks to localities whose partitions are ready.

#### Worker process

- Asynchronously wait for the partitions to become ready (instances created)
- Open dataset for writing
- Synchronously write hyperslabs corresponding to the worker's partitions from the LUE array into the HDF5
dataset. In case of parallel I/O, configure the HDF5 transfer mode as `HDF5D_MPIO_INDEPENDENT`.
