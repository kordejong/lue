import sys
from pathlib import Path

import docopt

import lue.framework_x as lfrx
from lue import __version__ as version


@lfrx.runtime_scope
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
    boolean_type = lfrx.boolean_element_type
    floating_point_type = lfrx.floating_point_element_types[0]

    condition = lfrx.as_field(
        lfrx.create_array((600, 400), lfrx.create_scalar(1, boolean_type))
    )
    x_coordinates = lfrx.cast(lfrx.cell_index(condition, 1), floating_point_type)
    y_coordinates = lfrx.cast(lfrx.cell_index(condition, 0), floating_point_type)
    result = lfrx.open_simplex_noise(x_coordinates, y_coordinates, seed=5)

    lfrx.to_gdal(result, str(result_array_path))
    # example]


if __name__ == "__main__":
    sys.exit(main())
