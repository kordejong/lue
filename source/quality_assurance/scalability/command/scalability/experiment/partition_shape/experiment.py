import os.path

from ...alias import Data, MutableData, Shape
from ..shape_range import ShapeRange
from .. import experiment


class Experiment(experiment.Experiment):
    def __init__(self, data: Data):
        super(Experiment, self).__init__(
            data,
            "partition_shape",
            data.get(
                "description",
                f"Partition size scalability experiment of {os.path.basename(data['command_pathname'])} "
                "for increasing problem sizes, "
                "relating runtime to increasing partition sizes",
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
        array_shape: Shape,
        basename: str,
        extension: str,
    ) -> str:
        return os.path.join(
            self.workspace_pathname(result_prefix, platform_name, scenario_name),
            "x".join([str(extent) for extent in array_shape]),
            "{}.{}".format(basename, extension),
        )
