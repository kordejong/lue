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

| Name | Description | Example |
| --- | ----- | --- |
| abs | | |
| acos | | |
| add | | |
| asin | | |
| atan | | |
| atan2 | | |
| ceil | | |
| close_to | | |
| cos | | |
| divide | | |
| equal_to | | |
| exp | | |
| floor | | |
| greater_than | | |
| greater_than_equal_to | | |
| less_than | | |
| less_than_equal_to | | |
| log | | |
| log10 | | |
| logical_and | | |
| logical_exclusive_or | | |
| logical_inclusive_or | | |
| logical_not | | |
| modulus | | |
| multiply | | |
| negate | | |
| not_equal_to | | |
| pow | | |
| round | | |
| sin | | |
| sqrt | | |
| subtract | | |
| tan | | |
| trunc | | |
| valid | | |
| where | | |

## Focal operations

Focal operations (AKA filter operations, neighbourhood operations, window operations) compute values for each
cell in the result array, given values within a kernel centered on each corresponding cell in the argument
array. Kernels have a shape and contain weights with which to multiply each corresponding cell in the argument
array. Focal operations differ with respect to how they compute a result value given argument values.

| Name | Description | Example |
| --- | ----- | --- |
| aspect | | |
| {py:func}`convolve` | Sum values in a neighbourhood, multiplied by weights | <img src="convolve/example-1/argument/array/5x5-float32.*" width="50"/> → <img src="convolve/example-1/result/cxx/convolve-1.*" width="50"/> |
| {py:func}`focal_diversity` | | |
| {py:func}`focal_high_pass` | | |
| {py:func}`focal_majority` | | |
| {py:func}`focal_maximum` | | |
| {py:func}`focal_mean` | | |
| {py:func}`focal_minimum` | | |
| {py:func}`focal_sum` | | |
| gradients | | |
| slope | | |

## Zonal operations

Zonal operations compute values for each cell in the result array, given corresponding cells in an argument
array. Each cell in the argument array belongs to a class, each of which defines a zone. Result values are
computed per zone, so cells in the result array located within the same zone receive the same value. Zonal
operations differ with respect to how they compute a result given the argument values per zone.

| Name | Description | Example |
| --- | ----- | --- |
| zonal_area | | |
| zonal_diversity | | |
| zonal_majority | | |
| zonal_maximum | | |
| zonal_mean | | |
| zonal_minimum | | |
| zonal_normal | | |
| zonal_sum | | |
| zonal_uniform | | |

## Global operations

Global operations compute a single result value, given all cell values in the input argument array. Global
operations differ with respect to how they compute a result given all values.

| Name | Description | Example |
| --- | ----- | --- |
| all | | |
| any | | |
| maximum | | |
| minimum | | |
| none | | |
| sum | | |

## Routing operations

| Name | Description | Example |
| --- | ----- | --- |
| {py:func}`d8_flow_direction` | Determine direction each cell drains towards | <img src="d8_flow_direction/example-1/argument/array/elevation-5x5-float32.*" width="50"/> → <img src="d8_flow_direction/example-1/result/cxx/d8_flow_direction-1.*" width="50"/> |
| {py:func}`downstream` | | |
| {py:func}`upstream` | | |
| accu | | |
| accu_capacity | | |
| accu_fraction | | |
| accu_info | | |
| accu_threshold | | |
| accu_trigger | | |
| decreasing_order | | |
| downstream_distance | | |
| first_n | | |
| inflow_count | | |
| kinematic_wave | | |
| partial_accu | | |
| upstream | | |

## Miscellaneous operations

| Name | Description | Example |
| --- | ----- | --- |
| {py:func}`open_simplex_noise` | Create noise field | <img src="open_simplex_noise/example-1/result/cxx/open_simplex_noise-1.*" width="50"/> |
| {py:func}`resample` | | |
| {py:func}`uniform` | | |
| array_like | | |
| array_partition_id | | |
| cast | | |
| cell_index | | |
| clump | | |
| copy | | |
| create_partitioned_array | | |
| fill | | |
| locality_id | | |
| mesh_grid | | |
| normal | | |
| partition_count_unique | | |
| range | | |
| reclassify | | |
| scalar | | |
| timestamp | | |
| unique | | |
| unique_id | | |
