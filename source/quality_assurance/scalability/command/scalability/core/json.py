import json
import os.path
from pathlib import Path

from ..alias import MutableData


__all__ = [
    "json_to_data",
]


def _json_to_data(path: Path) -> MutableData:
    lines = open(path).readlines()
    joined_lines = "".join([line for line in lines if not line.strip().startswith("#")])

    result: MutableData = json.loads(joined_lines)
    return result


def json_to_data(pathname: str) -> MutableData:
    """
    Read a JSON formatted file pointed to by `pathname` and return the contents as a Python dictionary

    - Any environment variables in `pathname` are expanded
    - Comments are supported: any line that starts with a pound sign (`#`) is skipped
    """
    pathname = os.path.expandvars(pathname)

    assert os.path.isfile(pathname), pathname

    return _json_to_data(Path(pathname))
