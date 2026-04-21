# `open_simplex_noise`

```{index} single: local operation; open_simplex_noise
```

## Signature

```{eval-rst}
.. py:function:: open_simplex_noise(x_coordinates, y_coordinates, seed) -> Field

    Return a noise field based on the OpenSimplex noise function

    :param Field x_coordinates: X-coordinates (array / floating point)
    :param Field y_coordinates: Y-coordinates (array / floating point)
    :param int seed: Seed
    :return: New field with values in the range [-1, 1] (array / floating point)
```

## Description

Operation for creating noise fields that can be used, for example, to generate synthetic elevation models.

The `cell_index` operation can be used to create the argument to pass into the operation.

## No-data handling

A cell containing a no-data value in one of the input arrays results in a no-data value in the corresponding
cell in the output array. No new no-data values are generated.

## Example

````{tab-set-code}

```{literalinclude} open_simplex_noise/example-1/open_simplex_noise.cpp
:language: c++
:start-after: [example
:end-before: example]
:dedent:
```

```{literalinclude} open_simplex_noise/example-1/open_simplex_noise.py
:language: python
:start-after: [example
:end-before: example]
:dedent:
```
````

| `result` |
| --- |
| ![result](open_simplex_noise/example-1/result/cxx/open_simplex_noise-1.*) |

Source code:
[C++](./open_simplex_noise/example-1/open_simplex_noise.cpp),
[Python](./open_simplex_noise/example-1/open_simplex_noise.py)

## See also

- [Wikipedia page](https://en.wikipedia.org/wiki/OpenSimplex_noise)
- [Blog post about making maps with noise functions](https://www.redblobgames.com/maps/terrain-from-noise/)
- [Algorithm used](https://github.com/Auburn/FastNoiseLite)
