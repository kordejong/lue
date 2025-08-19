"""
Implementation of the array_shape command-line utility
"""

import functools
import os.path
import sys

import docopt

from lue import __version__ as lue_version
from ..experiment.shape_range import array_shape


def main() -> None:
    """
    Main entry point of the command
    """
    usage = """\
Calculate array shape to use for scalability experiments

Usage:
    {command} <memory> <rank> <nr_arrays> <size_of_element> <nr_nodes>

Options:
    memory           Amount of memory to use per node [GiB == 2³⁰ bytes]
    rank             Rank of arrays
    nr_arrays        Number of arrays used in calculation
    size_of_element  Size of elements in arrays [bytes]
    nr_nodes         Number of nodes used in calculation
    -h --help        Show this screen

Wikipedia:
    In the context of computer memory, gigabyte and GB are customarily
    used to mean 1024³ (2³⁰) bytes.
""".format(command=os.path.basename(sys.argv[0]))

    arguments = docopt.docopt(usage, version=lue_version)

    memory_available = int(arguments["<memory>"])
    memory_available *= 2**30
    rank = int(arguments["<rank>"])
    nr_arrays = int(arguments["<nr_arrays>"])
    size_of_element = int(arguments["<size_of_element>"])
    nr_nodes = int(arguments["<nr_nodes>"])

    array_shape_ = array_shape(
        memory_available, rank, nr_arrays, size_of_element, nr_nodes
    )
    elements_per_array = functools.reduce(lambda e1, e2: e1 * e2, array_shape_)
    memory_required_per_array = elements_per_array * size_of_element
    memory_required = nr_arrays * memory_required_per_array

    assert memory_required - (nr_nodes * memory_available) < 1e-4

    # assert all([math.fmod(extent, nr_nodes) == 0 for extent in array_shape_])

    array_shape_per_node = tuple([float(extent) / nr_nodes for extent in array_shape_])

    assert all([isinstance(extent, float) for extent in array_shape_per_node])

    print(
        """\
shape (total): {}
shape (node): {}""".format(array_shape_, array_shape_per_node)
    )
