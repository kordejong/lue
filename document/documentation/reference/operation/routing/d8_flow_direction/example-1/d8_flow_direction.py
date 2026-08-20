import sys
from pathlib import Path

import docopt
import lue.framework as lfrx
from lue import __version__ as version


@lfrx.runtime_scope
def main() -> None:
    command = Path(sys.argv[0]).name
    usage = f"""\
Usage:
    {command} <elevation> <flow_direction>

Options:
    -h --help         Show this screen and exit
    --version         Show version and exit
    <elevation>       Elevation array to read
    <flow_direction>  Flow direction array to write
"""
    arguments = docopt.docopt(usage, sys.argv[1:], version=version)
    elevation_array_path = Path(arguments["<elevation>"])
    flow_direction_array_path = Path(arguments["<flow_direction>"])

    # [example
    elevation = lfrx.from_gdal(str(elevation_array_path))
    flow_direction = lfrx.d8_flow_direction(elevation)

    lfrx.to_gdal(flow_direction, str(flow_direction_array_path))
    # example]


if __name__ == "__main__":
    sys.exit(main())
