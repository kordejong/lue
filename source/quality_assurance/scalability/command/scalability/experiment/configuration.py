import os.path

from ..alias import Data
from ..core.json import json_to_data
from .benchmark import Benchmark
from .platform import Platform


class Configuration(object):
    def __init__(self, data: Data):
        self._from_data(data)

    def _from_data(self, data: Data) -> None:
        self.command_pathname = os.path.expandvars(data["command_pathname"])
        self.command_arguments = data["command_arguments"]
        self.script_pathname = os.path.expandvars(data["script_pathname"])
        self.result_prefix = os.path.expandvars(data["result_prefix"])
        self.platform = Platform(json_to_data(data["platform"]))
        self.benchmark = Benchmark(json_to_data(data["benchmark"]), self.platform)
