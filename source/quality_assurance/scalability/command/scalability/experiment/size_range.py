from abc import ABCMeta, abstractmethod
from typing import Any, Callable

from ..alias import Data, MutableData


class Range(metaclass=ABCMeta):
    """
    Base class for more concrete Range classes
    """

    min_size: int
    """
    Minimum size of range of sizes
    """

    max_size: int
    """
    Maximum size of range of sizes
    """

    def __init__(self, min_size: int, max_size: int):
        assert 0 < min_size <= max_size, f"0 < {min_size} <= {max_size}"

        self.min_size = min_size

        # Initialize max_size
        self.max_size = max_size

    def clamp_max_size(self) -> None:
        """
        Update max_size to make it equal to :py:func:`size` (:py:func:`nr_sizes` - 1)
        """
        self.max_size = self.size(self.nr_sizes - 1)

    @property
    @abstractmethod
    def nr_sizes(self) -> int:
        """
        Return the number of sizes in the range
        """

    @abstractmethod
    def size(self, idx: int) -> int:
        """
        Return the size at index `idx`, ranging from [0, :py:func:`nr_sizes` - 1).
        """


class MultiplyRange(Range):
    """
    Range class where sizes are computed by multiplying the minimum size until some maximum size is reached
    """

    multiplier: float
    """
    Multiplier for computing sizes
    """

    @classmethod
    def from_data(cls, data: Data) -> "MultiplyRange":
        min_size = data["min_size"]
        max_size = data["max_size"]
        multiplier = data["multiplier"]
        assert multiplier > 1, multiplier

        return cls(min_size, max_size, multiplier)

    @staticmethod
    def to_data(range: "MultiplyRange") -> MutableData:
        return {
            "min_size": range.min_size,
            "max_size": range.max_size,
            "multiplier": range.multiplier,
        }

    def __init__(self, min_size: int, max_size: int, multiplier: float):
        super(MultiplyRange, self).__init__(min_size, max_size)
        self.multiplier = multiplier
        self.clamp_max_size()

    def __str__(self) -> str:
        return "MultiplyRange(min_size={}, max_size={}, multiplier={})".format(
            self.min_size,
            self.max_size,
            self.multiplier,
        )

    @property
    def nr_sizes(self) -> int:
        size_ = float(self.min_size)
        nr = 1

        while self.multiplier * size_ <= self.max_size:
            size_ *= self.multiplier
            nr += 1

        return nr

    def size(self, idx: int) -> int:
        assert idx in range(self.nr_sizes), idx

        size_ = float(self.min_size)
        idx -= 1

        while idx >= 0:
            size_ *= self.multiplier
            idx -= 1

        return round(size_)


class IncrementRange(Range):
    """
    Range class where sizes are computed by adding an increment to the minimum size until some maximum size
    is reached
    """

    incrementor: int
    """
    Incrementor for computing sizes
    """

    @classmethod
    def from_data(cls, data: Data) -> "IncrementRange":
        min_size: int = data["min_size"]
        max_size: int = data["max_size"]
        incrementor: int = data["incrementor"]
        assert incrementor >= 1, incrementor

        return cls(min_size, max_size, incrementor)

    @staticmethod
    def to_data(range: "IncrementRange") -> MutableData:
        return {
            "min_size": range.min_size,
            "max_size": range.max_size,
            "incrementor": range.incrementor,
        }

    def __init__(self, min_size: int, max_size: int, incrementor: int):
        super(IncrementRange, self).__init__(min_size, max_size)
        self.incrementor = incrementor
        self.clamp_max_size()

    def __str__(self) -> str:
        return "IncrementRange(min_size={}, max_size={}, incrementor={})".format(
            self.min_size,
            self.max_size,
            self.incrementor,
        )

    @property
    def nr_sizes(self) -> int:
        size = self.min_size
        nr = 1

        size += self.incrementor

        while size <= self.max_size:
            size += self.incrementor
            nr += 1

        return nr

    def size(self, idx: int) -> int:
        assert idx in range(self.nr_sizes), idx

        size_ = self.min_size
        idx -= 1

        while idx >= 0:
            size_ += self.incrementor
            idx -= 1

        return size_


class ConstantRange(Range):
    """
    Range class containing a single size
    """

    @classmethod
    def from_data(cls, data: Data) -> "ConstantRange":
        size = data["size"]

        return cls(size)

    @staticmethod
    def to_data(range: "ConstantRange") -> MutableData:
        return {
            "size": range.min_size,
        }

    def __init__(self, size: int):
        super(ConstantRange, self).__init__(size, size)
        assert self.min_size == self.max_size

    def __str__(self) -> str:
        return "ConstantRange(size={})".format(self.min_size)

    @property
    def nr_sizes(self) -> int:
        return 1

    def size(self, idx: int) -> int:
        assert idx == 0
        return self.min_size


class SizeRange(object):
    """
    Class for describing a range of sizes

    A size is just an integer. A range of sizes is an range of increasing integral values. Multiple kinds
    of ranges can be used: :py:class:`MultiplyRange`, :py:class:`IncrementRange`, :py:class:`ConstantRange`.
    """

    range: Range
    """
    Object for computing sizes that are part of the range
    """

    @classmethod
    def from_data(cls, data: Data) -> "SizeRange":
        range: Range

        if "multiplier" in data:
            range = MultiplyRange.from_data(data)
        elif "incrementor" in data:
            range = IncrementRange.from_data(data)
        else:
            range = ConstantRange.from_data(data)

        return cls(range)

    @staticmethod
    def to_data(size_range: "SizeRange") -> MutableData:
        to_data_by_range: dict[type[Any], Callable[[Any], MutableData]] = {
            MultiplyRange: MultiplyRange.to_data,
            IncrementRange: IncrementRange.to_data,
            ConstantRange: ConstantRange.to_data,
        }
        return to_data_by_range[type(size_range.range)](size_range.range)

    def __init__(self, range: Range):
        self.range = range

    @property
    def min_size(self) -> int:
        """
        Return the minimum size
        """
        return self.range.min_size

    @property
    def max_size(self) -> int:
        """
        Return the maximum size
        """
        return self.range.max_size

    @property
    def nr_sizes(self) -> int:
        """
        Return the number of sizes in the range
        """
        return self.range.nr_sizes

    def size(self, idx: int) -> int:
        """
        Return the size at index `idx`, ranging from [0, :py:func:`nr_sizes` - 1).
        """
        return self.range.size(idx)
