# `uniform`

## Signature

```{eval-rst}
.. py:function:: uniform(array_shape, partition_shape, min_value, max_value) -> Field

   Return a field with values drawn from the uniform distribution

   :param Shape array_shape: Shape of the array
   :param Shape partition_shape: Shape of the partitions
   :param Field min_value: Minimum value of the distribution (value, scalar / arithmetic)
   :param Field max_value: Maximum value of the distribution (value, scalar / arithmetic)
   :return: New field with values in the range [min_value, max_value] (integral values) or [min_value,
        max_value) (floating point values) (array / arithmetic)

.. py:function:: uniform(array_shape, min_value, max_value) -> Field
   :no-index:

   Overload. A default partition shape will be used.

.. py:function:: uniform(array, min_value, max_value) -> Field
   :no-index:

   Overload. The passed in array's shape and partition shape will be used.

.. py:function:: uniform(min_value, max_value) -> Field

   Return a scalar with a value drawn from the uniform distribution.

   :param Scalar min_value: Minimum value of the distribution (value, scalar / arithmetic)
   :param Scalar max_value: Maximum value of the distribution (value, scalar / arithmetic)
   :return: New field with value in the range [min_value, max_value] (integral values) or [min_value,
        max_value) (floating point values) (array / arithmetic)
```

## Description

TODO

- max_value included / excluded vs integral / floating point values

## No-data handling

TODO

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
  - [Integral values](https://en.cppreference.com/cpp/numeric/random/uniform_int_distribution)
  - [Floating point values](https://en.cppreference.com/cpp/numeric/random/uniform_real_distribution)

| Software | Operation name |
| --- | --- |
| ArcGIS | TODO |
| PCRaster | `mapuniform` |
