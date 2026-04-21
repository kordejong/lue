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
    array_shape = (6, 4)
    min_value = 1
    max_value = 10
    result = lfr.uniform(array_shape, np.uint32, min_value, max_value)

    lfr.to_gdal(result, str(result_array_path))
    # example]


if __name__ == "__main__":
    sys.exit(main())
