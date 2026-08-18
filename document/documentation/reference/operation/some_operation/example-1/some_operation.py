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
    {command} <array> <result>

Options:
    -h --help   Show this screen and exit
    --version   Show version and exit
    <array>     Array to read
    <result>    Array to write
"""
    arguments = docopt.docopt(usage, sys.argv[1:], version=version)
    argument_array_path = Path(arguments["<array>"])
    result_array_path = Path(arguments["<result>"])

    # [example
    array = lfrx.from_gdal(str(argument_array_path))
    # TODO: Do something useful here
    result = array

    lfrx.to_gdal(result, str(result_array_path))
    # example]


if __name__ == "__main__":
    sys.exit(main())
