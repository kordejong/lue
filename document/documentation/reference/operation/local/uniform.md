# `uniform`

```{index} single: local operation; uniform
```

## Signature

```{eval-rst}
.. py:function::
    uniform(array_shape, partition_shape, min_value, max_value) -> Field
    uniform(array_shape, min_value, max_value) -> Field
    uniform(array, min_value, max_value) -> Field
    uniform(min_value, max_value) -> Field

    Return a field with value(s) drawn from the uniform distribution

    :param Shape array_shape: Shape of the array
    :param Array array: Array whose shape and partition shape will be used
    :param Shape partition_shape: Shape of the partitions. If not passed in a default partition shape will
        be used.
    :param Field min_value: Minimum value of the distribution (value, scalar / arithmetic)
    :param Field max_value: Maximum value of the distribution (value, scalar / arithmetic)
    :return: New field with value(s) in the range [min_value, max_value] (integral values) or
        [min_value, max_value) (floating point values) (array, scalar / arithmetic)

    .. note::
        Generating random values for `uint8` and `int8` element types is not supported.
```

## Description

Operation for creating random fields for various element types, with values in a custom range.

## No-data handling

If `min_value` or `max_value` is no-data, no-data is written to the result.

## Example

````{tab-set-code}

```{literalinclude} uniform/example-1/uniform.cpp
:language: c++
:start-after: [example
:end-before: example]
:dedent:
```

```{literalinclude} uniform/example-1/uniform.py
:language: python
:start-after: [example
:end-before: example]
:dedent:
```
````

| `result` |
| --- |
| ![result](uniform/example-1/result/cxx/uniform-1.*) |

Source code:
[C++](./uniform/example-1/uniform.cpp),
[Python](./uniform/example-1/uniform.py)

## See also

- Algorithms used:
  - [`std::uniform_int_distribution`](https://en.cppreference.com/cpp/numeric/random/uniform_int_distribution)
  for integral values
  - [`std::uniform_real_distribution`](https://en.cppreference.com/cpp/numeric/random/uniform_real_distribution)
    for floating point values

| Software | Operation name |
| --- | --- |
| ArcGIS | `UNIFORM` |
| NumPy | `random.uniform` |
| PCRaster | `mapuniform` |
