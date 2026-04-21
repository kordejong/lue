import sys
from pathlib import Path

import docopt
import numpy as np

import lue.document as ld
import lue.framework as lfr
from lue import __version__ as version


@lfr.runtime_scope
def main() -> None:
    command = Path(sys.argv[0]).name
    usage = f"""\
Usage:
    {command} <result>

Options:
    -h --help   Show this screen and exit
    --version   Show version and exit
    <result>    Array to write
"""
    arguments = docopt.docopt(usage, sys.argv[1:], version=version)
    result_array_path = Path(arguments["<result>"])

    # [example
    array_shape = (600, 400)
    condition = lfr.create_array(array_shape, lfr.boolean_element_type, 1)
    x_coordinates = lfr.cast(
        lfr.cell_index(condition, 1), lfr.floating_point_element_types[0]
    )
    y_coordinates = lfr.cast(
        lfr.cell_index(condition, 0), lfr.floating_point_element_types[0]
    )
    seed = 5
    result = lfr.open_simplex_noise(x_coordinates, y_coordinates, seed)

    lfr.to_gdal(result, str(result_array_path))
    # example]


if __name__ == "__main__":
    sys.exit(main())
