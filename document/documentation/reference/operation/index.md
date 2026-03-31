# Operations

```{note}
- Not all operations available are listed here already.
- The signatures of the operations are written in a language-agnostic manner. How to use each operation from a
  specific language might slightly differ. Check the usage examples for inspiration. Also, in Python the
  `help` function can be used to obtain more information (e.g.: `help(lfr.uniform)`).
```

## Local operations

Local operations compute values for each cell in the result array, given one or more corresponding
corresponding cells in one or more argument arrays. Local operations differ with respect to how they compute a
result value for a cell, given one or more argument values

## Focal operations

Focal operations (AKA filter operations, neighbourhood operations, window operations) compute values for each
cell in the result array, given values within a kernel centered on each corresponding cell in the argument
array. Kernels have a shape and contain weights with which to multiply each corresponding cell in the argument
array. Focal operations differ with respect to how they compute a result value given argument values.

## Zonal operations

Zonal operations compute values for each cell in the result array, given corresponding cells in an argument
array. Each cell in the argument array belongs to a class, each of which defines a zone. Result values are
computed per zone, so cells in the result array located within the same zone receive the same value. Zonal
operations differ with respect to how they compute a result given the argument values per zone.

## Global operations

Global operations compute a single result value, given all cell values in the input argument array. Global
operations differ with respect to how they compute a result given all values.

## Routing operations

TODO: description

## Miscellaneous operations

TODO: description

<!--
Connectivity
DefaultProgressor
DownscaleStrategy
DynamicModel
DynamicModelRunner
Hyperslab
Model
MonteCarloModel
MonteCarloModelRunner
PartitionedArray<float32, 2>
PartitionedArray<float64, 2>
PartitionedArray<int32, 2>
PartitionedArray<int64, 2>
PartitionedArray<int8, 2>
PartitionedArray<uint32, 2>
PartitionedArray<uint64, 2>
PartitionedArray<uint8, 2>
Progressor
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
SilentProgressor
StaticModel
StaticModelRunner
__builtins__
__cached__
__doc__
__file__
__loader__
__name__
__package__
__path__
__spec__
abs
accu
accu_capacity
accu_fraction
accu_info
accu_threshold
accu_trigger
acos
add
all
arithmetic_element_types
array_partition_id
asin
aspect
atan
atan2
boolean_element_type
cast
ceil
cell_index
clump
convolve
cos
count_element_type
create_array
create_scalar
d8_flow_direction
decreasing_order
default_hpx_runtime_configuration
divide
downscale
downstream
downstream_distance
equal_to
exp
first_n
floating_point_element_types
floor
flow_direction_element_type
focal_diversity
focal_high_pass
focal_majority
focal_maximum
focal_mean
focal_minimum
focal_sum
from_gdal
from_numpy
future<void>
greater_than
greater_than_equal_to
id_element_type
index_element_type
inflow_count
integral_element_types
iterate_per_element
kinematic_wave
less_than
less_than_equal_to
lfr
locality_id
log
log10
logical_and
logical_exclusive_or
logical_inclusive_or
logical_not
material_element_types
maximum
minimum
modulus
multiply
negate
normal
not_equal_to
on_root_locality
open_simplex_noise
partial_accu
pow
probe_gdal
read_array
reclassify
round
run_deterministic
run_stochastic
runtime_scope
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
signed_arithmetic_element_types
signed_integral_element_types
sin
slope
sqrt
start_hpx_runtime
stop_hpx_runtime
subtract
sum
support
tan
timestamp
to_gdal
to_numpy
trunc
uniform
unique_id
unsigned_integral_element_types
upstream
valid
where
with_development_operations
write_array
zonal_area
zonal_diversity
zonal_majority
zonal_maximum
zonal_mean
zonal_minimum
zonal_normal
zonal_sum
zonal_uniform
zone_element_types
-->
