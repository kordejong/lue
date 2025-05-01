import os.path

from ..shape import Shape
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

        self.from_json(data)

    def from_json(self, data):
        self.array = Shape.from_json(data["array"])
        self.partition = Shape.from_json(data["partition"])

    def to_json(self):
        result = super(Experiment, self).to_json()
        result["array"] = Shape.to_json(self.array)
        result["partition"] = Shape.to_json(self.partition)

        return result

    def benchmark_result_pathname(
        self,
        result_prefix,
        cluster_name,
        scenario_name,
        array_shape,
        basename,
        extension,
    ):
        return os.path.join(
            self.workspace_pathname(result_prefix, cluster_name, scenario_name),
            "x".join([str(extent) for extent in array_shape]),
            "{}.{}".format(basename, extension),
        )
