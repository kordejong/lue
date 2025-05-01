import os.path

from ..shape import Shape
from .. import experiment


class Experiment(experiment.Experiment):
    def __init__(self, data):
        super(Experiment, self).__init__(
            data,
            "strong_scalability",
            data.get(
                "description",
                "Strong scalability experiment of {}, "
                "relating runtime to an increasing number of workers, "
                "while keeping the total problem size fixed".format(
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
        self, result_prefix, cluster_name, scenario_name, nr_workers, extension
    ):
        return os.path.join(
            self.workspace_pathname(result_prefix, cluster_name, scenario_name),
            "{}.{}".format(nr_workers, extension),
        )
