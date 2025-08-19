from ...core.json import json_to_data
from .. import configuration
from . import experiment


class Configuration(configuration.Configuration):
    def __init__(self, data):
        super(Configuration, self).__init__(data)
        self.from_data(data)

    def from_data(self, data):
        experiment_data = json_to_data(data["experiment"])
        experiment_data["command_pathname"] = self.command_pathname
        experiment_data["command_arguments"] = self.command_arguments
        self.experiment = experiment.Experiment(experiment_data)
