import numpy as np
import numpy.typing as npt


def is_monotonically_increasing(array: npt.NDArray) -> bool:
    """
    Return whether `array` contains monotonically increasing values
    """
    # np.diff returns the difference between consecutive elements
    # Cast to float64 because when the type is unsigned int, this logic fails because a negative value
    # cannot be stored (out of range)

    return bool(np.all(np.diff(array.astype(np.float64)) > 0))
