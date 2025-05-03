import json

import numpy as np

import lue.data_model as ldm

from .benchmark import Benchmark
from .experiment import Experiment
from .platform import Platform


def write_benchmark_settings(
    lue_dataset: ldm.Dataset,
    platform: Platform,
    benchmark: Benchmark,
    experiment: Experiment,
) -> None:
    ldm.assert_is_valid(lue_dataset, fail_on_warning=False)

    phenomenon = lue_dataset.add_phenomenon("benchmark")
    property_set = phenomenon.add_collection_property_set("meta_information")

    platform_json = json.dumps(platform.to_data(), sort_keys=False, indent=4)
    platform_property = property_set.add_property(
        "platform_settings", dtype=np.dtype(np.str_)
    )
    platform_property.value.expand(1)[:] = np.array([platform_json.encode("utf8")])

    benchmark_json = json.dumps(benchmark.to_data(), sort_keys=False, indent=4)
    benchmark_property = property_set.add_property(
        "benchmark_settings", dtype=np.dtype(np.str_)
    )
    benchmark_property.value.expand(1)[:] = np.array([benchmark_json.encode("utf8")])

    experiment_json = json.dumps(experiment.to_data(), sort_keys=False, indent=4)
    experiment_property = property_set.add_property(
        "experiment_settings", dtype=np.dtype(np.str_)
    )
    experiment_property.value.expand(1)[:] = np.array([experiment_json.encode("utf8")])

    ldm.assert_is_valid(lue_dataset, fail_on_warning=False)


def write_script(lue_dataset: ldm.Dataset, script: str) -> None:
    ldm.assert_is_valid(lue_dataset)

    script_property = lue_dataset.benchmark.meta_information.add_property(
        "script", dtype=np.dtype(np.str_)
    )
    script_property.value.expand(1)[:] = np.array([script.encode("utf8")])

    ldm.assert_is_valid(lue_dataset)


def read_benchmark_settings(
    lue_dataset: ldm.Dataset, Experiment: type
) -> tuple[Platform, Benchmark, Experiment]:
    ldm.assert_is_valid(lue_dataset, fail_on_warning=False)

    phenomenon = lue_dataset.benchmark
    property_set = phenomenon.meta_information

    platform = Platform(json.loads(property_set.platform_settings.value[:][0]))
    benchmark = Benchmark(
        json.loads(property_set.benchmark_settings.value[:][0]), platform
    )
    experiment = Experiment(json.loads(property_set.experiment_settings.value[:][0]))

    return platform, benchmark, experiment


def raw_results_already_imported(lue_dataset: ldm.Dataset) -> bool:
    ldm.assert_is_valid(lue_dataset, fail_on_warning=False)

    # Test for something that is being imported
    return (
        "benchmark" in lue_dataset.phenomena
        and "meta_information" in lue_dataset.benchmark.collection_property_sets
        and "name" in lue_dataset.benchmark.meta_information.properties
    )
