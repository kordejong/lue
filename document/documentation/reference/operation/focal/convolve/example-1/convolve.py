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
    {command} <array> <kernel> <result>

Options:
    -h --help   Show this screen and exit
    --version   Show version and exit
    <array>     Array to read
    <kernel>    Kernel to read
    <result>    Array to write
"""
    arguments = docopt.docopt(usage, sys.argv[1:], version=version)
    argument_array_path = Path(arguments["<array>"])
    argument_kernel_path = Path(arguments["<kernel>"])
    result_array_path = Path(arguments["<result>"])

    array = ld.read_array(argument_array_path)
    kernel = ld.read_kernel(argument_kernel_path)

    # [example
    result = lfr.convolve(array, kernel)
    # example]

    lfr.to_gdal(result, str(result_array_path))


if __name__ == "__main__":
    sys.exit(main())
