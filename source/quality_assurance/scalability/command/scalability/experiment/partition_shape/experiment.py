import os.path

from ..shape_range import ShapeRange
from .. import experiment


class Experiment(experiment.Experiment):
    def __init__(self, data):
        super(Experiment, self).__init__(
            data,
            "partition_shape",
            data.get(
                "description",
                "Partition size scalability experiment of {} "
                "for increasing problem sizes, "
                "relating runtime to increasing partition sizes".format(
                    os.path.basename(data["command_pathname"])
                ),
            ),
        )

        self.from_data(data)

    def from_data(self, data):
        self.array = ShapeRange.from_data(data["array"])
        self.partition = ShapeRange.from_data(data["partition"])

    def to_data(self):
        result = super(Experiment, self).to_data()
        result["array"] = ShapeRange.to_data(self.array)
        result["partition"] = ShapeRange.to_data(self.partition)

        return result

    def benchmark_result_pathname(
        self,
        result_prefix,
        platform_name,
        scenario_name,
        array_shape,
        basename,
        extension,
    ):
        return os.path.join(
            self.workspace_pathname(result_prefix, platform_name, scenario_name),
            "x".join([str(extent) for extent in array_shape]),
            "{}.{}".format(basename, extension),
        )
