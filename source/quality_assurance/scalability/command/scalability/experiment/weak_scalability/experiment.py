import os.path

from ..shape_range import ShapeRange
from .. import experiment


class Experiment(experiment.Experiment):
    def __init__(self, data):
        super(Experiment, self).__init__(
            data,
            "weak_scalability",
            data.get(
                "description",
                "Weak scaling experiment of {}, "
                "relating runtime to an increasing number of workers, "
                "while keeping the problem size per worker constant".format(
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
