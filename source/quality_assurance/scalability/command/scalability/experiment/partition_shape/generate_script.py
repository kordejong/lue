import os.path

from .. import dataset, job
from .configuration import Configuration


def generate_script_slurm(
    result_prefix, platform, benchmark, experiment, script_pathname
):
    # Iterate over all combinations of array shapes and partition shapes
    # we need to benchmark and format a snippet of bash script for
    # executing the benchmark
    job_steps = []

    nr_localities = benchmark.worker.nr_localities

    srun_configuration = job.srun_configuration(platform)
    jobstarter = f"srun --ntasks {nr_localities} {srun_configuration}"

    for array_shape in experiment.array.shapes:
        result_workspace_pathname = os.path.join(
            experiment.workspace_pathname(
                result_prefix, platform.name, benchmark.scenario_name
            ),
            "x".join([str(extent) for extent in array_shape]),
        )

        job_steps += [
            # Create directory for the resulting json files
            f"mkdir -p {result_workspace_pathname}"
        ]

        for partition_shape in experiment.partition.shapes:
            result_pathname = experiment.benchmark_result_pathname(
                result_prefix,
                platform.name,
                benchmark.scenario_name,
                array_shape,
                "x".join([str(extent) for extent in partition_shape]),
                "json",
            )

            job_steps += [
                # Run the benchmark, resulting in a json file
                f"{jobstarter} {experiment.command_pathname} {experiment.command_arguments} "
                f'--hpx:threads="{benchmark.worker.nr_cores}" '
                "{program_configuration}".format(
                    program_configuration=job.program_configuration(
                        benchmark,
                        array_shape,
                        partition_shape,
                        result_pathname=result_pathname,
                        nr_workers=benchmark.worker.max_nr_workers,
                    ),
                ),
            ]

    slurm_script = job.create_slurm_script2(
        platform,
        nr_cluster_nodes=benchmark.worker.nr_cluster_nodes,
        nr_tasks=platform.nr_localities_to_reserve(
            benchmark.worker, benchmark.locality_per
        ),
        benchmark=benchmark,
        experiment=experiment,
        job_steps=job_steps,
        result_prefix=result_prefix,
    )

    job_name = "{name}-{program_name}".format(
        name=experiment.name, program_name=experiment.program_name
    )
    delimiter = "END_OF_SLURM_SCRIPT"

    commands = [
        "# Make sure SLURM can create the output file",
        "mkdir -p {}".format(
            experiment.workspace_pathname(
                result_prefix, platform.name, benchmark.scenario_name
            )
        ),
        "",
        "# Submit job to SLURM scheduler",
        "sbatch --job-name {job_name} {sbatch_options} << {delimiter}".format(
            job_name=job_name,
            sbatch_options=" ".join(platform.scheduler.settings.sbatch_options),
            delimiter=delimiter,
        ),
        slurm_script,
        "{delimiter}".format(delimiter=delimiter),
    ]

    job.write_script(commands, script_pathname)
    print("bash {}".format(script_pathname))


def generate_script_shell(
    result_prefix, platform, benchmark, experiment, script_pathname
):
    # Iterate over all combinations of array shapes and partition shapes
    # we need to benchmark and format a snippet of bash script for
    # executing the benchmark
    commands: list[str] = []

    for array_shape in experiment.array.shapes:
        for partition_shape in experiment.partition.shapes:
            result_pathname = experiment.benchmark_result_pathname(
                result_prefix,
                platform.name,
                benchmark.scenario_name,
                array_shape,
                "x".join([str(extent) for extent in partition_shape]),
                "json",
            )

            if not commands:
                commands += [
                    # Create directory for the resulting json file
                    "mkdir -p {}".format(os.path.dirname(result_pathname)),
                    "",
                ]

            commands += [
                # Run the benchmark, resulting in a json file
                f"{experiment.command_pathname} {experiment.command_arguments} "
                f'--hpx:threads="{benchmark.worker.nr_cores}" '
                "{program_configuration}".format(
                    program_configuration=job.program_configuration(
                        benchmark,
                        array_shape,
                        partition_shape,
                        result_pathname=result_pathname,
                        nr_workers=benchmark.worker.max_nr_workers,
                    ),
                ),
            ]

    job.write_script(commands, script_pathname)
    print("bash {}".format(script_pathname))


def generate_script(configuration_data):
    """
    Given a fixed set of workers, iterate over a range of array shapes
    and a range of partition shapes and capture benchmark results

    A shell script is created that submits jobs to the scheduler. Each
    job executes a benchmark and writes results to a JSON file.
    """
    configuration = Configuration(configuration_data)
    platform = configuration.platform
    benchmark = configuration.benchmark
    script_pathname = configuration.script_pathname
    result_prefix = configuration.result_prefix

    assert benchmark.worker.nr_benchmarks == 1
    assert not benchmark.worker.scale_over_cluster_nodes
    assert not benchmark.worker.scale_over_numa_nodes
    assert not benchmark.worker.scale_over_cores

    experiment = configuration.experiment

    lue_dataset = job.create_raw_lue_dataset(
        result_prefix, platform, benchmark, experiment
    )
    dataset.write_benchmark_settings(lue_dataset, platform, benchmark, experiment)

    if platform.scheduler.kind == "slurm":
        generate_script_slurm(
            result_prefix, platform, benchmark, experiment, script_pathname
        )
    elif platform.scheduler.kind == "shell":
        generate_script_shell(
            result_prefix, platform, benchmark, experiment, script_pathname
        )

    dataset.write_script(lue_dataset, open(script_pathname).read())
