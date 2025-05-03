from collections.abc import Sequence

from ..alias import Data, MutableData
from .worker import Worker


class Core(object):
    nr_threads: int

    def __init__(self, data: Data):
        self.from_data(data)

    def __str__(self) -> str:
        return "Core(nr_threads={})".format(
            self.nr_threads,
        )

    def from_data(self, data: Data) -> None:
        self.nr_threads = data["nr_threads"]

    def to_data(self) -> MutableData:
        return {
            "nr_threads": self.nr_threads,
        }


class NUMANode(object):
    memory: int
    nr_cores: int
    core: Core

    def __init__(self, data: Data):
        self.from_data(data)

    def __str__(self) -> str:
        return "NUMANode(memory={}, nr_cores={}, core={})".format(
            self.memory,
            self.nr_cores,
            self.core,
        )

    @property
    def nr_threads(self) -> int:
        return self.nr_cores * self.core.nr_threads

    def from_data(self, data: Data) -> None:
        self.memory = data["memory"]
        self.nr_cores = data["nr_cores"]
        self.core = Core(data["core"])

    def to_data(self) -> MutableData:
        return {
            "memory": self.memory,
            "nr_cores": self.nr_cores,
            "core": self.core.to_data(),
        }


class Package(object):
    _nr_numa_nodes: int
    numa_node: NUMANode

    def __init__(self, data: Data):
        self.from_data(data)

    def __str__(self) -> str:
        return "Package(nr_numa_nodes={}, numa_node={})".format(
            self._nr_numa_nodes,
            self.numa_node.to_data(),
        )

    @property
    def nr_numa_nodes(self) -> int:
        return self._nr_numa_nodes

    @property
    def memory(self) -> int:
        return self._nr_numa_nodes * self.numa_node.memory

    @property
    def nr_cores(self) -> int:
        return self._nr_numa_nodes * self.numa_node.nr_cores

    @property
    def nr_threads(self) -> int:
        return self._nr_numa_nodes * self.numa_node.nr_threads

    def from_data(self, data: Data) -> None:
        self._nr_numa_nodes = data["nr_numa_nodes"]
        self.numa_node = NUMANode(data["numa_node"])

    def to_data(self) -> MutableData:
        return {
            "nr_numa_nodes": self.nr_numa_nodes,
            "numa_node": self.numa_node.to_data(),
        }


class ClusterNode(object):
    nr_packages: int
    package: Package

    def __init__(self, data: Data):
        self.from_data(data)

    def __str__(self) -> str:
        return "ClusterNode(nr_packages={}, package={})".format(
            self.nr_packages,
            self.package,
        )

    @property
    def nr_numa_nodes(self) -> int:
        return self.nr_packages * self.package.nr_numa_nodes

    @property
    def memory(self) -> int:
        return self.nr_packages * self.package.memory

    @property
    def nr_cores(self) -> int:
        return self.nr_packages * self.package.nr_cores

    @property
    def nr_threads(self) -> int:
        return self.nr_packages * self.package.nr_threads

    def from_data(self, data: Data) -> None:
        self.nr_packages = data["nr_packages"]
        self.package = Package(data["package"])

    def to_data(self) -> MutableData:
        return {
            "nr_packages": self.nr_packages,
            "package": self.package.to_data(),
        }


class Scheduler(object):
    kind: str

    def __init__(self, data: Data):
        self._from_data(data)

    def _from_data(self, data: Data) -> None:
        self.kind = data["kind"]

    def to_data(self) -> MutableData:
        return {"kind": self.kind}


class ShellScheduler(Scheduler):
    def __init__(self, data: Data):
        super(ShellScheduler, self).__init__(data)

    def to_data(self) -> MutableData:
        return super(ShellScheduler, self).to_data()


class SlurmSettings(object):
    partition_name: str
    sbatch_options: Sequence[str]
    mpirun_options: Sequence[str]
    srun_options: Sequence[str]

    def __init__(self, data: Data):
        self.from_data(data)

    def from_data(self, data: Data) -> None:
        self.partition_name = data["partition"]
        self.sbatch_options = data["sbatch_options"] if "sbatch_options" in data else ""
        self.mpirun_options = data["mpirun_options"] if "mpirun_options" in data else ""
        self.srun_options = data["srun_options"] if "srun_options" in data else ""

    def to_data(self) -> MutableData:
        result: MutableData = {
            "partition": self.partition_name,
        }

        if self.sbatch_options:
            result["sbatch_options"] = self.sbatch_options

        if self.mpirun_options:
            result["mpirun_options"] = self.mpirun_options

        if self.srun_options:
            result["srun_options"] = self.srun_options

        return result


class SlurmScheduler(Scheduler):
    settings: SlurmSettings

    def __init__(self, data: Data):
        super(SlurmScheduler, self).__init__(data)
        self.from_data(data)

    def from_data(self, data: Data) -> None:
        self.settings = SlurmSettings(data["settings"])

    def to_data(self) -> MutableData:
        result = super(SlurmScheduler, self).to_data()

        result["settings"] = self.settings.to_data()

        return result


class SoftwareEnvironment(object):
    module_names: Sequence[str]
    venv: Data | None

    def __init__(self, data: Data):
        self.from_data(data)

    def from_data(self, data: Data) -> None:
        self.module_names = data["modules"]

        if "venv" in data:
            self.venv = data["venv"]
        else:
            self.venv = None

    def to_data(self) -> MutableData:
        result: MutableData = {
            "modules": self.module_names,
        }

        if self.venv is not None:
            result["venv"] = self.venv

        return result

    @property
    def configuration(self) -> str:
        commands = []

        if self.module_names:
            commands = ["module purge"]

        commands += ["module load {}".format(name) for name in self.module_names]

        if self.venv:
            environment = self.venv["environment"]
            commands.append(f"source {environment}/bin/activate")

        return "\n".join(commands)


class Platform(object):
    name: str
    scheduler: Scheduler
    nr_cluster_nodes: int
    cluster_node: ClusterNode
    software_environment: SoftwareEnvironment | None

    def __init__(self, data: Data):
        self.from_data(data)

    def from_data(self, data: Data) -> None:
        self.name = data["name"]
        scheduler_json = data["scheduler"]
        scheduler_kind = scheduler_json["kind"]

        if scheduler_kind == "shell":
            self.scheduler = ShellScheduler(scheduler_json)
        elif scheduler_kind == "slurm":
            self.scheduler = SlurmScheduler(scheduler_json)

        self.nr_cluster_nodes = data["nr_cluster_nodes"]
        self.cluster_node = ClusterNode(data["cluster_node"])

        self.software_environment = (
            SoftwareEnvironment(data["software_environment"])
            if "software_environment" in data
            else None
        )

    def to_data(self) -> MutableData:
        result = {
            "name": self.name,
            "scheduler": self.scheduler.to_data(),
            "nr_cluster_nodes": self.nr_cluster_nodes,
            "cluster_node": self.cluster_node.to_data(),
        }

        if self.software_environment:
            result["software_environment"] = self.software_environment.to_data()

        return result

    def nr_localities_to_reserve(self, worker: Worker, locality_per: str) -> int:
        """
        When scheduling jobs, the number of localities we need to ask for from the scheduler

        This number might be larger than the actual number of localities used by tasks. We want to prevent to
        compete with other jobs while performing the benchmarks.
        """
        result = -1

        if worker.name == "core":
            # Claim a whole cluster node
            if locality_per == "cluster_node":
                result = 1
            elif locality_per == "numa_node":
                result = self.cluster_node.nr_numa_nodes
        elif worker.name == "numa_node":
            # Claim a whole cluster node
            assert locality_per == "numa_node"
            result = self.cluster_node.nr_numa_nodes
        elif worker.name == "cluster_node":
            # Claim all cluster nodes in the partition
            if locality_per == "cluster_node":
                result = worker.nr_cluster_nodes
            elif locality_per == "numa_node":
                result = worker.nr_cluster_nodes * self.cluster_node.nr_numa_nodes

        assert result > 0

        return result
