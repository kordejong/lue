"""
In the context of the scalability command these terms have the following meaning:

- Benchmark: Aggregate for information about the experiment: hardware, workers to use, ...
- Experiment: One of three scalability experiments: partition shape, strong scalability, weak scalability
- Task: One of three experiment tasks: script, import, postprocess
- Run: A computational task whose latency is recorded
- Count: Number of times an experimental run should be repeated
"""

from .alias import Data
from .experiment import partition_shape, strong_scalability, weak_scalability
from .query import export_duration, summarize_experiment


__all__ = [
    "export_duration",
    "perform_experiment_task",
    "summarize_experiment",
]


def perform_experiment_task(
    experiment_name: str, task_name: str, configuration_data: Data
) -> None:
    experiment_by_name = {
        "partition_shape": partition_shape,
        "weak_scalability": weak_scalability,
        "strong_scalability": strong_scalability,
    }

    experiment_by_name[experiment_name].perform_task(task_name, configuration_data)
