import sys
from pathlib import Path

import docopt

import lue.document as ld
import lue.framework as lfr
from lue import __version__ as version


@lfr.runtime_scope
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

    elevation = ld.read_array(elevation_array_path)

    # [example
    flow_direction = lfr.d8_flow_direction(elevation)
    # example]

    lfr.to_gdal(flow_direction, str(flow_direction_array_path))


if __name__ == "__main__":
    sys.exit(main())
