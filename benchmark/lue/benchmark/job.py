import os.path


### def output_pathname(
###         job_script_pathname):
### 
###     return "{}.out".format(job_script_pathname)

###     # template = "{}-{{}}.out".format(job_script_pathname)
### 
###     # i = 0
###     # result = template.format(i)
### 
###     # while os.path.exists(result):
###     #     i += 1
###     #     result = template.format(i)
### 
###     # return result


def configuration(
        cluster,
        benchmark,
        experiment,
        array_shape,
        partition_shape,
        result_pathname):

    return \
        '--hpx:ini="application.{program_name}.benchmark.cluster_name!={cluster_name}" ' \
        '--hpx:ini="application.{program_name}.benchmark.count!={count}" ' \
        '--hpx:ini="application.{program_name}.benchmark.max_tree_depth!={max_tree_depth}" ' \
        '--hpx:ini="application.{program_name}.benchmark.output!={result_pathname}" ' \
        '--hpx:ini="application.{program_name}.nr_time_steps!={nr_time_steps}" ' \
        '--hpx:ini="application.{program_name}.array_shape!={array_shape}" ' \
        '--hpx:ini="application.{program_name}.partition_shape!={partition_shape}" ' \
            .format(
                program_name=experiment.program_name,
                cluster_name=cluster.name,
                count=benchmark.count,
                max_tree_depth=experiment.max_tree_depth,
                nr_time_steps=experiment.nr_time_steps,
                array_shape=list(array_shape),
                partition_shape=list(partition_shape),
                result_pathname=result_pathname,
            )


def write_script(
        commands,
        script_pathname):

    with open(script_pathname, "w") as script:
        script.write("""\
#!/usr/bin/env bash
set -e

{commands}
""".format(
            commands="\n".join(commands)
        ))


def create_slurm_script(
        nr_nodes,
        nr_threads,
        output_filename,
        partition_name,
        max_duration,
        job_steps):

    # HPX doc:
    # You can change the number of localities started per node
    # (for example to account for NUMA effects) by specifying the
    # -n option of srun. The number of cores per locality can be
    # set by -c.

    # There is no need to use any of the HPX command line options
    # related to the number of localities, number of threads,
    # or related to networking ports. All of this information is
    # automatically extracted from the SLURM environment by the
    # HPX startup code.

    # The srun documentation explicitly states: "If -c is
    # specified without -n as many tasks will be allocated per node
    # as possible while satisfying the -c restriction. For instance
    # on a cluster with 8 CPUs per node, a job request for 4 nodes
    # and 3 CPUs per task may be allocated 3 or 6 CPUs per node (1
    # or 2 tasks per node) depending upon resource consumption by
    # other jobs." For this reason, we suggest to always specify
    # -n <number-of-instances>, even if <number-of-instances>
    # is equal to one (1).

    return """\
#!/usr/bin/env bash
#SBATCH --nodes={nr_nodes}
#SBATCH --ntasks={nr_nodes}
#SBATCH --cpus-per-task={nr_threads}
#SBATCH --output={output_filename}
#SBATCH --partition={partition_name}
{max_duration}

set -e

module purge
module load userspace/all
module load gcc/7.2.0
module load boost/gcc72/1.65.1
module load mpich/gcc72/mlnx/3.2.1

{job_steps}""".format(
        nr_nodes=nr_nodes,
        nr_threads=nr_threads,
        output_filename=output_filename,
        partition_name=partition_name,
        max_duration="#SBATCH --time={}".format(max_duration)
            if max_duration is not None else "",
        job_steps="\n".join(job_steps))