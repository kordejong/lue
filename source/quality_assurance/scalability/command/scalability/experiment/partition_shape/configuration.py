from ...core.json import json_to_data
from ...alias import Data
from .. import configuration
from .experiment import Experiment


class Configuration(configuration.Configuration):
    experiment: Experiment

    def __init__(self, data: Data):
        super(Configuration, self).__init__(data)
        self.from_data(data)

    def from_data(self, data: Data) -> None:
        experiment_data = json_to_data(data["experiment"])
        experiment_data["command_pathname"] = self.command_pathname
        experiment_data["command_arguments"] = self.command_arguments
        self.experiment = Experiment(experiment_data)
