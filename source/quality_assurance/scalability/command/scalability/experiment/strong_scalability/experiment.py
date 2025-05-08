import os.path

from ...alias import Data, MutableData
from ..shape_range import ShapeRange
from .. import experiment


class Experiment(experiment.Experiment):
    array: ShapeRange
    partition: ShapeRange

    def __init__(self, data: Data):
        super(Experiment, self).__init__(
            data,
            "strong_scalability",
            data.get(
                "description",
                f"Strong scalability experiment of {os.path.basename(data['command_pathname'])}, "
                "relating runtime to an increasing number of workers, "
                "while keeping the total problem size fixed",
            ),
        )

        self.from_data(data)

    def from_data(self, data: Data) -> None:
        self.array = ShapeRange.from_data(data["array"])
        self.partition = ShapeRange.from_data(data["partition"])

    def to_data(self) -> MutableData:
        result = super(Experiment, self).to_data()
        result["array"] = ShapeRange.to_data(self.array)
        result["partition"] = ShapeRange.to_data(self.partition)

        return result

    def benchmark_result_pathname(
        self,
        result_prefix: str,
        platform_name: str,
        scenario_name: str,
        nr_workers: int,
        extension: str,
    ) -> str:
        return os.path.join(
            self.workspace_pathname(result_prefix, platform_name, scenario_name),
            "{}.{}".format(nr_workers, extension),
        )
