import sys
from pathlib import Path

import docopt

from lue import __version__ as version

from ..data import import_example_data
from .main import main_function


@main_function
def main() -> None:
    """Command line interface for command."""
    command = Path(sys.argv[0]).name
    usage = f"""\
Import example data from text file into LUE JSON

Usage:
    {command} <text> <json> <meta>

Options:
    -h --help   Show this screen and exit
    --version   Show version and exit
    <text>      Path to text file to import
    <json>      Path to JSON file to write
    <meta>      Path to JSON metadata file to write

The JSON file can be used lue_translate to import the data into a LUE dataset.
The metadata JSON file can be used by lue_translate to export the data to
another format. See the lue_translate documentation for more information.
"""
    arguments = docopt.docopt(usage, sys.argv[1:], version=version)
    dataset_path = Path(arguments["<text>"])
    json_path = Path(arguments["<json>"])
    meta_path = Path(arguments["<meta>"])

    import_example_data(dataset_path, json_path, meta_path)
