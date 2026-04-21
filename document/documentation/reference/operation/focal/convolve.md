# `convolve`

```{index} single: focal operation; convolve
```

## Signature

```{eval-rst}
.. py:function:: convolve(field, kernel) -> Field

    Sum all values in a neighbourhood, multiplied by their weights

    :param Field field: Field to analyse (array / floating point)
    :param Kernel kernel: Neighbourhood to search. The weights must be floating point and will be used to
        multiply each cell's value with.
    :return: New field (array / floating point)
```

## Description

Focal operation summing all values in a neighbourhood, multiplied by their weights.

Formula for computing a result for a single cell {math}`(row, col)`, given an array {math}`A` and a 3x3
kernel {math}`K`:

```{math}
:label: convolve
\sum_{i=-1}^{1} \sum_{j=-1}^{1} K[i][j] \times A[row+i][col+j]
```

## No-data handling

As long as there is at least one valid value found within the input neighbourhood, a valid value is written to
the focal cell in the output field. Only when no such value is found is a no-data value written. The output
field is likely to contain less no-data values than the input field.

## Example

````{tab-set-code}

```{literalinclude} convolve/example-1/convolve.cpp
:language: c++
:start-after: [example
:end-before: example]
:dedent:
```

```{literalinclude} convolve/example-1/convolve.py
:language: python
:start-after: [example
:end-before: example]
:dedent:
```

````

| `array` | `kernel` | `result` |
| --- | --- | --- |
| ![array](convolve/example-1/argument/array/5x5-float32.*) | ![kernel](convolve/example-1/argument/kernel/3x3-float32.*) | ![result](convolve/example-1/result/cxx/convolve-1.*) |

Source code:
[C++](./convolve/example-1/convolve.cpp),
[Python](./convolve/example-1/convolve.py)

## See also

- See {py:func}`focal_sum` for an operation which sums values without multiplying them by weights
