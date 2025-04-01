import os.path

from .. import dataset, job
from ..worker import Worker
from .configuration import Configuration


def nr_workers(worker: Worker) -> int:
    """
    Return the number of individual represented by the Worker instance passed in
    """
    result = -1

    if worker.type == "thread":
        result = worker.nr_threads
    elif worker.type == "numa_node":
        result = worker.nr_numa_nodes
    elif worker.type == "cluster_node":
        result = worker.nr_cluster_nodes

    return result


def generate_script_slurm_threads(
    result_prefix, cluster, benchmark, experiment, script_pathname
):
    # - Reserve a single NUMA node, including all its memory
    # - Iterate over all partition shapes
    # - Perform an experiment

    # Iterate over all combinations of array shapes and partition shapes we need to benchmark and format a
    # snippet of bash script for executing the benchmark

    job_steps = []

    nr_localities = benchmark.worker.nr_localities
    assert nr_localities == 1, nr_localities

    srun_configuration = job.srun_configuration(cluster)
    jobstarter = f"srun --ntasks {nr_localities} {srun_configuration}"

    for array_shape in experiment.array.shapes:
        result_workspace_pathname = os.path.join(
            experiment.workspace_pathname(
                result_prefix, cluster.name, benchmark.scenario_name
            ),
            "x".join([str(extent) for extent in array_shape]),
        )

        job_steps += [
            # Create directory for the resulting json files
            f"srun --ntasks 1 mkdir -p {result_workspace_pathname}"
        ]

        for partition_shape in experiment.partition.shapes:
            result_pathname = experiment.benchmark_result_pathname(
                result_prefix,
                cluster.name,
                benchmark.scenario_name,
                array_shape,
                "x".join([str(extent) for extent in partition_shape]),
                "json",
            )

            job_steps += [
                # Run the benchmark, resulting in a json file
                f"{jobstarter} {experiment.command_pathname} {experiment.command_arguments} "
                f'--hpx:threads="{benchmark.worker.nr_threads}" '
                "{program_configuration}".format(
                    program_configuration=job.program_configuration(
                        result_prefix,
                        cluster,
                        benchmark,
                        experiment,
                        array_shape,
                        partition_shape,
                        result_pathname=result_pathname,
                        nr_workers=nr_workers(benchmark.worker),
                    ),
                ),
            ]

    # Here:
    # - number of SLURM tasks == number of HPX localities == number of OS processes == number of NUMA nodes
    # - number of NUMA nodes needed == 1
    # - number of cores per task is number of real cores per NUMA node
    # - thread binding must be in "hardware order"
    # - amount of memory needed is amount available in the one NUMA node

    # TODO resources are not shared yet

    slurm_script = job.create_slurm_script2(
        cluster, benchmark, experiment, job_steps, result_prefix
    )

    job_name = "{name}-{program_name}".format(
        name=experiment.name, program_name=experiment.program_name
    )
    delimiter = "END_OF_SLURM_SCRIPT"

    commands = [
        "# Make sure SLURM can create the output file",
        "mkdir -p {}".format(
            experiment.workspace_pathname(
                result_prefix, cluster.name, benchmark.scenario_name
            )
        ),
        "",
        "# Submit job to SLURM scheduler",
        "sbatch --job-name {job_name} {sbatch_options} << {delimiter}".format(
            job_name=job_name,
            sbatch_options=" ".join(cluster.scheduler.settings.sbatch_options),
            delimiter=delimiter,
        ),
        slurm_script,
        "{delimiter}".format(delimiter=delimiter),
    ]

    job.write_script(commands, script_pathname)
    print("bash {}".format(script_pathname))


def generate_script_slurm_numa_nodes(
    result_prefix, cluster, benchmark, experiment, script_pathname
):
    assert False


def generate_script_slurm_cluster_nodes(
    result_prefix, cluster, benchmark, experiment, script_pathname
):
    assert False


def generate_script_shell(
    result_prefix, cluster, benchmark, experiment, script_pathname
):
    assert benchmark.worker.type == "thread"

    # Iterate over all combinations of array shapes and partition shapes
    # we need to benchmark and format a snippet of bash script for
    # executing the benchmark
    commands = []

    nr_threads = benchmark.worker.nr_threads

    for array_shape in experiment.array.shapes:
        for partition_shape in experiment.partition.shapes:
            result_pathname = experiment.benchmark_result_pathname(
                result_prefix,
                cluster.name,
                benchmark.scenario_name,
                array_shape,
                "x".join([str(extent) for extent in partition_shape]),
                "json",
            )

            commands += [
                # Create directory for the resulting json file
                "mkdir -p {}".format(os.path.dirname(result_pathname)),
                # Run the benchmark, resulting in a json file
                f"{experiment.command_pathname} {experiment.command_arguments} "
                f'--hpx:threads="{nr_threads}" '
                "{program_configuration}".format(
                    program_configuration=job.program_configuration(
                        result_prefix,
                        cluster,
                        benchmark,
                        experiment,
                        array_shape,
                        partition_shape,
                        result_pathname=result_pathname,
                        nr_workers=nr_workers(benchmark.worker),
                    ),
                ),
            ]

    job.write_script(commands, script_pathname)
    print("bash {}".format(script_pathname))


def generate_script_slurm(
    result_prefix, cluster, benchmark, experiment, script_pathname
):
    assert benchmark.worker.type in ["cluster_node", "numa_node", "thread"]

    if benchmark.worker.type == "cluster_node":
        generate_script_slurm_cluster_nodes(
            result_prefix, cluster, benchmark, experiment, script_pathname
        )
    elif benchmark.worker.type == "numa_node":
        generate_script_slurm_numa_nodes(
            result_prefix, cluster, benchmark, experiment, script_pathname
        )
    elif benchmark.worker.type == "thread":
        generate_script_slurm_threads(
            result_prefix, cluster, benchmark, experiment, script_pathname
        )


def generate_script(configuration_data):
    """
    Given a fixed set of workers, iterate over a range of array shapes
    and a range of partition shapes and capture benchmark results

    A shell script is created that submits jobs to the scheduler. Each
    job executes a benchmark and writes results to a JSON file.
    """
    configuration = Configuration(configuration_data)
    cluster = configuration.cluster
    benchmark = configuration.benchmark
    script_pathname = configuration.script_pathname
    result_prefix = configuration.result_prefix

    assert benchmark.worker.nr_benchmarks == 1
    assert benchmark.worker.nr_cluster_nodes_range == 0
    assert benchmark.worker.nr_numa_nodes_range == 0
    assert benchmark.worker.nr_threads_range == 0

    experiment = configuration.experiment

    lue_dataset = job.create_raw_lue_dataset(
        result_prefix, cluster, benchmark, experiment
    )
    dataset.write_benchmark_settings(lue_dataset, cluster, benchmark, experiment)

    if cluster.scheduler.kind == "slurm":
        generate_script_slurm(
            result_prefix, cluster, benchmark, experiment, script_pathname
        )
    elif cluster.scheduler.kind == "shell":
        generate_script_shell(
            result_prefix, cluster, benchmark, experiment, script_pathname
        )

    dataset.write_script(lue_dataset, open(script_pathname).read())
