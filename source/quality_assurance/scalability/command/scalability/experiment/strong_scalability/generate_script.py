import os.path

from .. import dataset, job
from .configuration import Configuration


def generate_script_slurm_cores(
    result_prefix, platform, benchmark, experiment, script_pathname
):
    """
    Scale over cores in a single NUMA node or a single cluster node
    """
    assert not benchmark.worker.scale_over_cluster_nodes
    assert not benchmark.worker.scale_over_numa_nodes
    assert benchmark.worker.scale_over_cores

    job_steps = []
    array_shape = experiment.array.shape
    partition_shape = experiment.partition.shape
    nr_localities = 1  # Single NUMA node or cluster node
    srun_configuration = job.srun_configuration(platform)
    jobstarter = f"srun --ntasks {nr_localities} {srun_configuration}"

    for benchmark_idx in range(benchmark.worker.nr_benchmarks):
        nr_workers = benchmark.worker.nr_workers(benchmark_idx)
        nr_cores = nr_workers

        job_steps += [
            # Run the benchmark, resulting in a json file
            f"{jobstarter} {experiment.command_pathname} {experiment.command_arguments} "
            f'--hpx:threads="{nr_cores}" '
            "{program_configuration}".format(
                program_configuration=job.program_configuration(
                    benchmark,
                    array_shape,
                    partition_shape if nr_workers > 1 else array_shape,
                    result_pathname=experiment.benchmark_result_pathname(
                        result_prefix,
                        platform.name,
                        benchmark.scenario_name,
                        nr_workers,
                        "json",
                    ),
                    nr_workers=nr_workers,
                ),
            )
        ]

    slurm_script = job.create_slurm_script2(
        platform,
        nr_cluster_nodes=1,
        nr_tasks=1,
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


def generate_script_slurm_numa_nodes(
    result_prefix, platform, benchmark, experiment, script_pathname
):
    """
    Scale over NUMA nodes in a single cluster node
    """
    assert not benchmark.worker.scale_over_cluster_nodes
    assert benchmark.worker.scale_over_numa_nodes
    assert not benchmark.worker.scale_over_cores

    commands = [
        "mkdir -p {}".format(
            experiment.workspace_pathname(
                result_prefix, platform.name, benchmark.scenario_name
            )
        ),
    ]
    array_shape = experiment.array.shape
    partition_shape = experiment.partition.shape

    nr_cores = benchmark.worker.nr_cores
    srun_configuration = job.srun_configuration(platform)

    for benchmark_idx in range(benchmark.worker.nr_benchmarks):
        nr_workers = benchmark.worker.nr_workers(benchmark_idx)
        nr_localities = nr_workers
        jobstarter = f"srun --ntasks {nr_localities} {srun_configuration}"

        job_steps = [
            f"{jobstarter} {experiment.command_pathname} {experiment.command_arguments} "
            f'--hpx:threads="{nr_cores}" '
            "{program_configuration}".format(
                program_configuration=job.program_configuration(
                    benchmark,
                    array_shape,
                    partition_shape,
                    result_pathname=experiment.benchmark_result_pathname(
                        result_prefix,
                        platform.name,
                        benchmark.scenario_name,
                        nr_workers,
                        "json",
                    ),
                    nr_workers=nr_workers,
                ),
            )
        ]

        slurm_script = job.create_slurm_script2(
            platform,
            nr_cluster_nodes=1,
            nr_tasks=nr_localities,
            benchmark=benchmark,
            experiment=experiment,
            job_steps=job_steps,
            result_prefix=result_prefix,
        )

        job_name = "{name}-{program_name}-{nr_workers}".format(
            name=experiment.name,
            program_name=experiment.program_name,
            nr_workers=nr_workers,
        )
        delimiter = "END_OF_SLURM_SCRIPT_{}".format(nr_workers)

        commands += [
            "",
            "sbatch --job-name {job_name} {sbatch_options} << {delimiter}".format(
                job_name=job_name,
                sbatch_options=" ".join(platform.scheduler.settings.sbatch_options),
                delimiter=delimiter,
            ),
            slurm_script,
            "{delimiter}".format(delimiter=delimiter),
            "",
            # Prevent benchmarks to start at the same time point
            "sleep 3s",
        ]

    job.write_script(commands, script_pathname)
    print("bash {}".format(script_pathname))


def generate_script_slurm_cluster_nodes(
    result_prefix, platform, benchmark, experiment, script_pathname
):
    """
    Scale over nodes in a cluster of nodes
    """
    assert benchmark.worker.scale_over_cluster_nodes
    assert not benchmark.worker.scale_over_numa_nodes
    assert not benchmark.worker.scale_over_cores

    # Given a problem size, measure the duration of executing an
    # executable on increasingly more nodes. For each number of nodes
    # to use, we create a SLURM script containing the job steps for that
    # partition. Then we create a shell script that executes the SLURM
    # scripts, passing jobs to the scheduler.

    # Iterate over all sets of workers we need to benchmark and format
    # a snippet of bash script for executing the benchmark

    commands = []
    array_shape = experiment.array.shape
    partition_shape = experiment.partition.shape

    nr_cores = benchmark.worker.nr_cores
    srun_configuration = job.srun_configuration(platform)

    for benchmark_idx in range(benchmark.worker.nr_benchmarks):
        nr_workers = benchmark.worker.nr_workers(benchmark_idx)
        nr_localities = nr_workers * benchmark.worker.nr_numa_nodes
        jobstarter = f"srun --ntasks {nr_localities} {srun_configuration}"

        job_steps = [
            # Run the benchmark, resulting in a json file
            f"{jobstarter} {experiment.command_pathname} {experiment.command_arguments} "
            f'--hpx:threads="{nr_cores}" '
            "{program_configuration}".format(
                program_configuration=job.program_configuration(
                    benchmark,
                    array_shape,
                    partition_shape,
                    result_pathname=experiment.benchmark_result_pathname(
                        result_prefix,
                        platform.name,
                        benchmark.scenario_name,
                        nr_workers,
                        "json",
                    ),
                    nr_workers=nr_workers,
                ),
            )
        ]

        slurm_script = job.create_slurm_script2(
            platform,
            nr_cluster_nodes=nr_workers,
            nr_tasks=nr_localities,
            benchmark=benchmark,
            experiment=experiment,
            job_steps=job_steps,
            result_prefix=result_prefix,
        )

        # slurm_script = job.create_slurm_script(
        #     platform,
        #     nr_cluster_nodes=nr_workers,
        #     nr_tasks=nr_localities,
        #     nr_cores_per_socket=platform.cluster_node.package.numa_node.nr_cores,
        #     cpus_per_task=benchmark.nr_logical_cores_per_locality,
        #     output_filename=experiment.benchmark_result_pathname(
        #         result_prefix, platform.name, benchmark.scenario_name, nr_workers, "out"
        #     ),
        #     partition_name=platform.scheduler.settings.partition_name,
        #     sbatch_options=platform.scheduler.settings.sbatch_options,
        #     max_duration=experiment.max_duration,
        #     job_steps=job_steps,
        # )

        job_name = "{name}-{program_name}-{nr_workers}".format(
            name=experiment.name,
            program_name=experiment.program_name,
            nr_workers=nr_workers,
        )
        delimiter = "END_OF_SLURM_SCRIPT_{}".format(nr_workers)

        result_pathname = experiment.benchmark_result_pathname(
            result_prefix, platform.name, benchmark.scenario_name, nr_workers, "json"
        )

        commands += [
            # Create a snippet of bash script that creates a SLURM script
            # for this partition. Note that this has to be done at runtime
            # because there is no directory structure yet for storing
            # SLURM scripts.
            "# Number of nodes: {}".format(nr_workers),
            # Create directory for storing SLURM script and benchmark
            # results
            "mkdir -p {}".format(os.path.dirname(result_pathname)),
            # Submit SLURM script to scheduler
            "sbatch --job-name {job_name} {sbatch_options} << {delimiter}".format(
                job_name=job_name,
                sbatch_options=" ".join(platform.scheduler.settings.sbatch_options),
                delimiter=delimiter,
            ),
            slurm_script,
            "{delimiter}".format(delimiter=delimiter),
            "",
            # Prevent benchmarks to start at the same time point
            "sleep 3s",
            "",
        ]

    job.write_script(commands, script_pathname)
    print("bash {}".format(script_pathname))


def generate_script_slurm(
    result_prefix, platform, benchmark, experiment, script_pathname
):
    assert benchmark.worker.name in ["cluster_node", "numa_node", "core"]

    if benchmark.worker.scale_over_cluster_nodes:
        generate_script_slurm_cluster_nodes(
            result_prefix, platform, benchmark, experiment, script_pathname
        )
    elif benchmark.worker.scale_over_numa_nodes:
        generate_script_slurm_numa_nodes(
            result_prefix, platform, benchmark, experiment, script_pathname
        )
    elif benchmark.worker.scale_over_cores:
        generate_script_slurm_cores(
            result_prefix, platform, benchmark, experiment, script_pathname
        )


def generate_script_shell(
    result_prefix, platform, benchmark, experiment, script_pathname
):
    assert benchmark.worker.name == "core"
    assert not benchmark.worker.scale_over_cluster_nodes
    assert not benchmark.worker.scale_over_numa_nodes
    assert benchmark.worker.scale_over_cores

    # Iterate over all sets of workers we need to benchmark and format
    # a snippet of bash script for executing the benchmark
    commands: list[str] = []

    array_shape = experiment.array.shape
    partition_shape = experiment.partition.shape

    for benchmark_idx in range(benchmark.worker.nr_benchmarks):
        nr_workers = benchmark.worker.nr_workers(benchmark_idx)
        nr_cores = nr_workers
        result_pathname = experiment.benchmark_result_pathname(
            result_prefix, platform.name, benchmark.scenario_name, nr_workers, "json"
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
            f'--hpx:threads="{nr_cores}" '
            "{program_configuration}".format(
                program_configuration=job.program_configuration(
                    benchmark,
                    array_shape,
                    partition_shape if nr_workers > 1 else array_shape,
                    result_pathname=experiment.benchmark_result_pathname(
                        result_prefix,
                        platform.name,
                        benchmark.scenario_name,
                        nr_workers,
                        "json",
                    ),
                    nr_workers=nr_workers,
                ),
            ),
        ]

    job.write_script(commands, script_pathname)
    print("bash {}".format(script_pathname))


def generate_script(configuration_data):
    """
    Given a fixed array size and partition shape size, iterate over a
    range of sets of workers and capture benchmark results

    A shell script is created that submits jobs to the scheduler. Each
    job executes a benchmark and writes results to a JSON file.
    """
    configuration = Configuration(configuration_data)
    platform = configuration.platform
    benchmark = configuration.benchmark
    script_pathname = configuration.script_pathname
    result_prefix = configuration.result_prefix

    assert (
        sum(
            [
                benchmark.worker.scale_over_cluster_nodes,
                benchmark.worker.scale_over_numa_nodes,
                benchmark.worker.scale_over_cores,
            ]
        )
        == 1
    )
    assert benchmark.worker.nr_benchmarks > 0

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
