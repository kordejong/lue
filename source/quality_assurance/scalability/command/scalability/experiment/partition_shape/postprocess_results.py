from collections.abc import Sequence
import dateutil.parser
import matplotlib.pyplot as plt
import matplotlib.axes as mpla
import numpy as np

import lue.data_model as ldm

from ...alias import Data, Shapes
from ...core import math, plot, process
from .. import dataset, job
from .configuration import Configuration
from .experiment import Experiment


def post_process_raw_results(
    lue_dataset: ldm.Dataset,
    result_prefix: str,
    plot_pathname: str,
    experiment: Experiment,
    filetypes: Sequence[str],
) -> None:
    """
    Create plots and tables from raw benchmark results
    """
    lue_meta_information = lue_dataset.benchmark.meta_information
    name = lue_meta_information.name.value[:][0]
    system_name = lue_meta_information.system_name.value[:][0]
    scenario_name = lue_meta_information.scenario_name.value[:][0]

    count = lue_dataset.benchmark.measurement.duration.value.shape[1]

    # The time point at which the experiment was performed is the epoch
    # of the time domain used to store the durations
    lue_clock = lue_dataset.benchmark.measurement.time_domain.clock
    assert lue_clock.nr_units == 1
    time_point_units = lue_clock.unit

    lue_epoch = lue_clock.epoch
    assert lue_epoch.kind == ldm.Epoch.Kind.common_era
    assert lue_epoch.calendar == ldm.Calendar.gregorian
    time_point = dateutil.parser.isoparse(lue_epoch.origin).strftime("%c")

    array_shapes = lue_dataset.array.array.shape.value[:]
    assert len(array_shapes) == len(array_shapes)
    assert np.all(array_shapes == array_shapes)

    partition_shapes = lue_dataset.partition.partition.shape.value[:]
    partition_sizes = np.prod(partition_shapes, axis=1)
    assert len(partition_shapes) == len(partition_sizes)
    # nr_partitions = len(partition_shapes)

    assert math.is_monotonically_increasing(partition_sizes), partition_sizes

    def plot_duration(
        axis: mpla.Axes,
        array_idx: int,
        partition_sizes: Sequence[int],
        partition_shapes: Shapes,
    ) -> None:
        if count == 1:
            duration = lue_dataset.partition.partition.properties[
                "duration_{}".format(array_idx)
            ].value[:]
            y_label = "duration ({})".format(time_point_units)
            axis.plot(
                partition_sizes,
                duration,
                linewidth=plot.default_linewidth,
                color=plot.actual_color,
                marker="o",
            )
        else:
            duration = lue_dataset.partition.partition.properties[
                "mean_duration_{}".format(array_idx)
            ].value[:]
            error = lue_dataset.partition.partition.properties[
                "std_duration_{}".format(array_idx)
            ].value[:]
            y_label = "duration ({}) ± stddev (count={})".format(
                time_point_units, count
            )
            axis.errorbar(
                x=partition_sizes,
                y=duration,
                yerr=error,
                linewidth=plot.default_linewidth,
                color=plot.actual_color,
                marker="o",
            )

        axis.set_xlabel("partition shape")
        axis.set_xticks(partition_sizes)
        axis.set_xticklabels(
            [
                "{}".format(
                    "{}x{} ({size})".format(
                        *partition_shapes[i], size=partition_sizes[i]
                    )
                )
                for i in range(len(partition_shapes))
            ],
            rotation=45,
        )

        axis.set_ylabel(y_label)

        axis.grid()

    # --------------------------------------------------------------------------
    # For each array shape (size) a line plot with, for a given array
    # size, the spread of durations per partition size
    # for array_shape in array_shapes:
    for array_idx in range(len(array_shapes)):
        array_shape = array_shapes[array_idx]

        figure, axis = plt.subplots(figsize=(15, 10))  # Inches...

        plot_duration(axis, array_idx, partition_sizes, partition_shapes)

        # TODO For current array shape, plot nr_partitions by partition shape
        # plot_nr_partitions(axis, array_idx)

        figure.suptitle(
            "{}, {}, {}\n"
            "Partition shape scalability experiment on {} array ({})".format(
                name,
                system_name,
                time_point,
                "x".join([str(extent) for extent in array_shape]),
                scenario_name,
            )
        )

        for filetype in filetypes:
            a_plot_pathname = experiment.benchmark_result_pathname(
                result_prefix, system_name, scenario_name, array_shape, "plot", filetype
            )
            plt.savefig(a_plot_pathname, bbox_inches="tight", transparent=True)

    # --------------------------------------------------------------------------
    array_shapes = lue_dataset.array.array.shape.value[:]
    array_sizes = np.prod(array_shapes, axis=1)
    assert len(array_shapes) == len(array_sizes)
    nr_arrays = len(array_shapes)
    nr_plots = nr_arrays

    figure, axes = plt.subplots(
        figsize=(15, nr_plots * 10), nrows=nr_plots, ncols=1, sharex=True, sharey=True
    )

    if nr_arrays == 1:
        axes = [axes]

    for array_idx in range(nr_arrays):
        axis = axes[array_idx]
        plot_duration(axis, array_idx, partition_sizes, partition_shapes)
        axis.set_title(
            "{}x{} ({size})".format(
                *array_shapes[array_idx], size=array_sizes[array_idx]
            )
        )

    figure.suptitle(
        "{}, {}, {}\nPartition shape scalability experiment ({})".format(
            name,
            system_name,
            time_point,
            scenario_name,
        )
    )

    for filetype in filetypes:
        plt.savefig(
            f"{plot_pathname}.{filetype}", bbox_inches="tight", transparent=True
        )


def postprocess_results(configuration_data: Data) -> None:
    configuration = Configuration(configuration_data)
    platform = configuration.platform
    benchmark = configuration.benchmark
    result_prefix = configuration.result_prefix
    experiment: Experiment = configuration.experiment

    lue_dataset = job.open_scalability_lue_dataset(
        result_prefix, platform, benchmark, experiment, "r"
    )
    platform, benchmark, experiment = dataset.read_benchmark_settings(
        lue_dataset, Experiment
    )

    filetypes = ["pdf", "png", "svg"]

    process.create_dot_graph(
        lue_dataset.pathname,
        experiment.result_pathname(
            result_prefix, platform.name, benchmark.scenario_name, "graph", "pdf"
        ),
    )
    plot_pathname = experiment.result_pathname(
        result_prefix, platform.name, benchmark.scenario_name, "plot"
    )
    post_process_raw_results(
        lue_dataset, result_prefix, plot_pathname, experiment, filetypes
    )
