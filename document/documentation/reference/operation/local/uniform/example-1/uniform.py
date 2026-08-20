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
    dtype = lfrx.signed_integral_element_types[-1]
    result = lfrx.uniform(
        (6, 4),
        min_value=lfrx.as_field(lfrx.create_literal(1, dtype)),
        max_value=lfrx.as_field(lfrx.create_literal(10, dtype)),
    )

    lfrx.to_gdal(result, str(result_array_path))
    # example]


if __name__ == "__main__":
    sys.exit(main())
