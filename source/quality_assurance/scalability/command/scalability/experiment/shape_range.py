from collections.abc import Iterable, Sequence
from functools import reduce
import math

from ..alias import Data, MutableData, Shape, Shapes


def array_shape(
    memory_available: int,
    rank: int,
    nr_arrays: int,
    size_of_element: int,
    nr_nodes: int,
) -> Shape:
    """
    Determine a maximum array shape to use in scaling experiments, given the
    amount of memory to use, and properties of the arrays

    memory_available: bytes per node
    rank: int
    nr_arrays: int
    size_of_element: bytes
    nr_nodes: int
    """
    assert memory_available > 0
    assert rank > 0
    assert nr_arrays > 0
    assert size_of_element > 0
    assert nr_nodes > 0

    nr_elements = float(nr_nodes * memory_available) / size_of_element
    elements_per_array = nr_elements / nr_arrays
    elements_per_dimension = math.pow(elements_per_array, 1.0 / rank)

    elements_per_dimension = int(math.floor(elements_per_dimension))

    return rank * (elements_per_dimension,)


def partition_shape_multipliers(shape: Shape, partition_shape: Shape) -> Sequence[int]:
    """
    Given an array shape and a partition shape, determine for each dimension how many times the partition's
    extent can fit in the shape's extent.

    Returns a list of multipliers: for each dimension one.
    """

    rank = len(shape)
    assert len(partition_shape) == rank

    assert all([extent > 0 for extent in shape])
    assert all([extent > 0 for extent in partition_shape])

    for r in range(rank):
        assert shape[r] % partition_shape[r] == 0

    shape_multipliers = [shape[r] // partition_shape[r] for r in range(rank)]

    assert all([isinstance(multiplier, int) for multiplier in shape_multipliers])

    return shape_multipliers


def ranges_of_partition_shape_multipliers(
    shape: Shape, min_partition_shape: Shape, max_partition_shape: Shape
) -> Sequence[Sequence[int]]:
    """
    Return for each dimension a range of multipliers
    """
    min_partition_shape_multipliers = partition_shape_multipliers(
        shape, min_partition_shape
    )
    max_partition_shape_multipliers = partition_shape_multipliers(
        shape, max_partition_shape
    )

    rank = len(shape)

    assert all(
        [
            min_partition_shape_multipliers[r] >= max_partition_shape_multipliers[r]
            for r in range(rank)
        ]
    )

    multiplier_ranges = [
        range(
            min_partition_shape_multipliers[r], max_partition_shape_multipliers[r], -1
        )
        for r in range(rank)
    ]

    assert len(multiplier_ranges) == rank

    return multiplier_ranges


def shape_ranges(
    min_shape: Shape, max_shape: Shape, step: int
) -> Sequence[Sequence[int]]:
    assert len(min_shape) == len(max_shape)
    assert step > 0
    rank = len(min_shape)
    assert rank > 0
    assert all([min_shape[r] <= max_shape[r] for r in range(rank)])

    return [range(min_shape[r], max_shape[r] + 1, step) for r in range(rank)]


def partition_shapes(min_shape: Shape, max_shape: Shape, step: int) -> Iterable[Shape]:
    # TODO Rename this function. Related to shapes, not partition shapes

    shape_ranges_ = shape_ranges(min_shape, max_shape, step)

    return zip(*shape_ranges_)


def range_of_shapes(
    min_shape: Shape, max_nr_elements: int, multiplier: int, method: str
) -> Shapes:
    """
    Determine a range of shapes given the following requirements:

    - First shape equals min_shape
    - Number of cells in last shape smaller or equal to max_nr_elements
    - Each next shape contains multiplier times more elements than the previous shape
    """

    def nr_elements(shape: Shape) -> int:
        return reduce(lambda e1, e2: e1 * e2, shape)

    def shape(nr_elements: int, normalized_shape: Sequence[float]) -> Shape:
        rank = len(normalized_shape)
        nr_elements_per_dimension = nr_elements ** (1.0 / rank)

        return tuple(
            [
                int(math.floor(nr_elements_per_dimension * extent))
                for extent in normalized_shape
            ]
        )

    def linear_increment(idx: int, size: int, multiplier: int) -> int:
        # Increase the number of cells linearly, by idx * (multiplier - 1) * size
        # If idx == 0, size is returned
        return size + idx * (multiplier - 1) * size

    sizes = [nr_elements(min_shape)]

    assert method in ["linear", "exponential"]

    assert multiplier > 1, multiplier

    if method == "linear":
        idx = 1
        new_size = linear_increment(idx, sizes[0], multiplier)

        while new_size <= max_nr_elements:
            sizes.append(new_size)
            idx += 1
            new_size = linear_increment(idx, sizes[0], multiplier)

    elif method == "exponential":
        # Increase the number of cells exponentially, by multiplier *
        # previous number of elements
        while multiplier * sizes[-1] <= max_nr_elements:
            sizes.append(multiplier * sizes[-1])

    normalized_shape = tuple([float(extent) / max(min_shape) for extent in min_shape])
    shapes = list(shape(size, normalized_shape) for size in sizes)

    assert all(nr_elements(shape) <= max_nr_elements for shape in shapes), (
        f"{shapes}, {max_nr_elements}".format(shapes, max_nr_elements)
    )

    return shapes


class Range(object):
    max_nr_elements: int
    multiplier: int
    method: str

    def __init__(self, data: Data):
        self.from_data(data)

    def from_data(self, data: Data) -> None:
        self.max_nr_elements = data["max_nr_elements"]
        self.multiplier = data["multiplier"]
        assert isinstance(self.multiplier, int)  # Otherwise update type hints
        self.method = data["method"]

        assert self.method in ["linear", "exponential"]

        if self.method == "linear":
            assert self.multiplier > 1

    def to_data(self) -> MutableData:
        return {
            "max_nr_elements": self.max_nr_elements,
            "multiplier": self.multiplier,
            "method": self.method,
        }


class ShapeRange(object):
    _shape: Shape
    _range: Range | None

    @classmethod
    def from_data(cls, data: Data) -> "ShapeRange":
        shape = tuple(data["shape"])
        range = Range(data["range"]) if "range" in data else None

        return cls(shape, range)

    @staticmethod
    def to_data(shape_range: "ShapeRange") -> MutableData:
        result: MutableData = {"shape": shape_range._shape}

        if shape_range._range:
            result["range"] = shape_range._range.to_data()

        return result

    def __init__(self, shape: Shape, range: Range | None):
        self._shape = shape
        self._range = range

    def is_fixed(self) -> bool:
        return self._range is None

    @property
    def shapes(self) -> Shapes:
        result: Shapes

        if self.is_fixed():
            result = [self._shape]
        else:
            # Range of shapes
            assert self._range is not None
            result = range_of_shapes(
                self._shape,
                self._range.max_nr_elements,
                self._range.multiplier,
                self._range.method,
            )

        return result

    @property
    def shape(self) -> Shape:
        """
        Return shape

        It is assumed that this instance represents a fixed shape,
        not a range of shapes.
        """
        assert self.is_fixed()

        return self._shape
