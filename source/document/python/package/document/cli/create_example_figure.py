import sys
from pathlib import Path

import docopt

from lue import __version__ as version

from ..figure import plot
from .main import main_function


@main_function
def main() -> None:
    """Command line interface for command."""
    command = Path(sys.argv[0]).name
    usage = f"""\
Create a figure given an example dataset

Usage:
    {command} <dataset> <figure> <format>...

Options:
    -h --help   Show this screen and exit
    --version   Show version and exit
    <dataset>   Path to dataset to read
    <figure>    Path to figure to write, without file extension
    <format>... Formats to write
"""
    arguments = docopt.docopt(usage, sys.argv[1:], version=version)
    dataset_path = Path(arguments["<dataset>"])
    figure_path = Path(arguments["<figure>"])
    formats = arguments["<format>"]

    plot(dataset_path, figure_path, formats)
