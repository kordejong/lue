# `d8_flow_direction`

```{index} single: routing operation; d8_flow_direction
```

## Signature

```{eval-rst}
.. py:function:: lue.framework.d8_flow_direction(elevation) -> Field

   Determine direction each cell drains towards

   :param Field elevation: Floating point array
   :return: New integral array
```

## Description

Focal operation for determining the direction each cell drains towards. Directions are encoded by an integral
number. Each number represents one of eight flow directions and the sink "direction":

| code | direction |
| --- | --- |
| 7 | north-west |
| 8 | ↑ north |
| 9 | north-east |
| 4 | ← west |
| 5 | ■ sink |
| 6 | → east |
| 1 | south-west |
| 2 | ↓ south |
| 3 | south-east |

## No-data handling

A cell containing a no-data value in the input array results in a no-data value in the corresponding output
array. No new no-data values are generated.

## Example

````{tab-set-code}

```{literalinclude} d8_flow_direction/example-1/d8_flow_direction.cpp
:language: c++
:start-after: [example
:end-before: example]
:dedent:
```

```{literalinclude} d8_flow_direction/example-1/d8_flow_direction.py
:language: python
:start-after: [example
:end-before: example]
:dedent:
```

````

| `elevation` | `flow_direction` |
| --- | --- |
| ![elevation](d8_flow_direction/example-1/argument/array/elevation-5x5-float32.*) | ![flow_direction](d8_flow_direction/example-1/result/cxx/d8_flow_direction-1.*) |

Source code:
[C++](./d8_flow_direction/example-1/d8_flow_direction.cpp),
[Python](./d8_flow_direction/example-1/d8_flow_direction.py)
