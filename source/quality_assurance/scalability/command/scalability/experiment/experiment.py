from abc import ABCMeta  # , abstractmethod
from collections.abc import Mapping, Sequence
import os.path

from ..alias import Data, MutableData


class Arguments(object):
    positionals: Sequence[str]
    options: Mapping[str, str]

    def __init__(self, data: Data):
        self.positionals = data["positionals"] if "positionals" in data else []
        self.options = data["options"] if "options" in data else {}

    @property
    def to_list(self) -> Sequence[str]:
        result = []

        for positional in self.positionals:
            result.append('"{}"'.format(positional))

        for key, value in self.options.items():
            result.append(
                '--{}="{}"'.format(key, value) if value else "--{}".format(key)
            )

        return result


class Experiment(metaclass=ABCMeta):
    name: str
    description: str
    command_pathname: str
    command_arguments: str
    arguments: Arguments
    max_duration: int | None
    max_tree_depth: int | None
    program_name: str

    def __init__(self, data: Data, name: str, description: str):
        # Name (kind) of the experiment
        self.name = name

        self.description = description
        self.command_pathname = data["command_pathname"]
        self.command_arguments = data["command_arguments"]
        self.arguments = (
            Arguments(data["arguments"]) if "arguments" in data else Arguments({})
        )

        self.max_duration = data["max_duration"] if "max_duration" in data else None
        self.max_tree_depth = (
            data["max_tree_depth"] if "max_tree_depth" in data else None
        )
        # self.nr_time_steps = data["nr_time_steps"]

        self.program_name = os.path.basename(self.command_pathname)

    def to_data(self) -> MutableData:
        result: MutableData = {
            "command_pathname": self.command_pathname,
            "command_arguments": self.command_arguments,
            # "nr_time_steps": self.nr_time_steps,
        }

        if self.max_duration:
            result["max_duration"] = self.max_duration

        if self.max_tree_depth:
            result["max_tree_depth"] = self.max_tree_depth

        return result

    @property
    def argument_list(self) -> Sequence[str]:
        return self.arguments.to_list

    def workspace_pathname(
        self, result_prefix: str, platform_name: str, scenario_name: str
    ) -> str:
        """
        Return pathname of directory in which or below which all experiment results must be stored
        """
        return os.path.join(
            os.path.abspath(result_prefix),
            platform_name,
            self.program_name,
            scenario_name,
            self.name,
        )

    def result_pathname(
        self,
        result_prefix: str,
        platform_name: str,
        scenario_name: str,
        basename: str,
        extension: str = "",
    ) -> str:
        return os.path.join(
            self.workspace_pathname(result_prefix, platform_name, scenario_name),
            f"{basename}.{extension}" if extension else basename,
        )

    # @abstractmethod
    # def benchmark_result_pathname(
    #     self, result_prefix, platform_name, scenario_name, nr_workers, extension
    # ):
    #     """
    #     Return pathname
    #     """
