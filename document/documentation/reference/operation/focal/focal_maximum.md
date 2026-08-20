# `focal_maximum`

```{index} single: focal operation; focal_maximum
```

## Signature

```{eval-rst}
.. py:function:: focal_maximum(array, kernel) -> Field

   Determine the maximum value in a neighbourhood

   :param Field array: Arithmetic array to analyse
   :param Kernel kernel: Neighbourhood to search. The weights must be integral and will be used to determine
        whether a cell's value is to be evaluated (weight is non-zero) or not (weight is zero).
   :return: New arithmetic array
```

## Description

Focal operation for determining the maximum value in a neighbourhood.

## No-data handling

As long as there is at least one valid value found within the input neighbourhood, a valid value is written to
the focal cell in the output array. Only when no such value is found is a no-data value written. The output
array is likely to contain less no-data values than the input array.

## Example

````{tab-set-code}

```{code-block} c
/* TODO */
```

```{code-block} c++
// TODO
```

```{code-block} python
# TODO
```

````
