from abc import ABCMeta, abstractmethod
import typing

from .cluster import Cluster
from .size_range import SizeRange


__all__ = ["Worker"]


class WorkerBase(metaclass=ABCMeta):
    name = ""

    def __init__(self, size_range: SizeRange, locality_per: str):
        self.locality_per = locality_per
        self.size_range = size_range

        self.min_nr_cores = 1
        self.max_nr_cores = 1
        self.min_nr_numa_nodes = 1
        self.max_nr_numa_nodes = 1
        self.min_nr_cluster_nodes = 1
        self.max_nr_cluster_nodes = 1

    @property
    def nr_cores_range(self) -> int:
        return self.max_nr_cores - self.min_nr_cores + 1

    @property
    def nr_numa_nodes_range(self) -> int:
        return self.max_nr_numa_nodes - self.min_nr_numa_nodes + 1

    @property
    def nr_cluster_nodes_range(self) -> int:
        return self.max_nr_cluster_nodes - self.min_nr_cluster_nodes + 1

    @property
    def nr_cores(self) -> int:
        assert self.nr_cores_range == 1

        return self.min_nr_cores

    @property
    def nr_numa_nodes(self) -> int:
        assert self.nr_numa_nodes_range == 1

        return self.min_nr_numa_nodes

    @property
    def nr_cluster_nodes(self) -> int:
        assert self.nr_cluster_nodes_range == 1

        return self.min_nr_cluster_nodes

    @property
    @abstractmethod
    def nr_localities(self) -> int:
        """
        Return number of localities to use in benchmark
        """

    @property
    @abstractmethod
    def max_nr_workers(self) -> int:
        """
        Return the maximum number of workers to use
        """


class Core(WorkerBase):
    name = "core"

    def __init__(self, size_range: SizeRange, cluster: Cluster, locality_per: str):
        super().__init__(size_range, locality_per)

        # Range of CPU cores for scaling
        self.min_nr_cores = self.size_range.min_size
        self.max_nr_cores = self.size_range.max_size

        assert self.locality_per in ["numa_node", "cluster_node"], self.locality_per

        if self.locality_per == "numa_node":
            # Per the one NUMA node locality
            self.min_nr_numa_nodes = 1
        elif self.locality_per == "cluster_node":
            # Per the one cluster node locality
            self.min_nr_numa_nodes = cluster.cluster_node.nr_numa_nodes

        self.max_nr_numa_nodes = self.min_nr_numa_nodes

    @property
    def max_nr_workers(self) -> int:
        return self.nr_cores

    @property
    def nr_localities(self) -> int:
        assert self.nr_numa_nodes_range == 1
        assert self.nr_cluster_nodes_range == 1

        return 1


class NUMANode(WorkerBase):
    name = "numa_node"

    def __init__(self, size_range: SizeRange, cluster: Cluster, locality_per: str):
        super().__init__(size_range, locality_per)

        assert self.locality_per == "numa_node", self.locality_per

        # Per each of the NUMA node localities
        self.min_nr_cores = cluster.cluster_node.package.numa_node.nr_cores
        self.max_nr_cores = self.min_nr_cores

        # Range of NUMA nodes for scaling
        self.min_nr_numa_nodes = self.size_range.min_size
        self.max_nr_numa_nodes = self.size_range.max_size

    @property
    def max_nr_workers(self) -> int:
        return self.nr_numa_nodes

    @property
    def nr_localities(self) -> int:
        assert self.locality_per == "numa_node", self.locality_per
        assert self.nr_numa_nodes_range == 1
        assert self.nr_cluster_nodes_range == 1

        return self.min_nr_numa_nodes


class ClusterNode(WorkerBase):
    name = "cluster_node"

    def __init__(self, size_range: SizeRange, cluster: Cluster, locality_per: str):
        super().__init__(size_range, locality_per)

        assert self.locality_per == "numa_node", self.locality_per

        # Per each of the NUMA node localities
        self.min_nr_cores = cluster.cluster_node.package.numa_node.nr_cores
        self.max_nr_cores = self.min_nr_cores

        # All NUMA node localities
        self.min_nr_numa_nodes = cluster.cluster_node.nr_numa_nodes
        self.max_nr_numa_nodes = self.min_nr_numa_nodes

        # Range of cluster nodes for scaling
        self.min_nr_cluster_nodes = self.size_range.min_size
        self.max_nr_cluster_nodes = self.size_range.max_size

    @property
    def max_nr_workers(self) -> int:
        return self.nr_cluster_nodes

    @property
    def nr_localities(self) -> int:
        assert self.locality_per == "numa_node", self.locality_per
        assert self.nr_numa_nodes_range == 1
        assert self.nr_cluster_nodes_range == 1

        return self.min_nr_cluster_nodes * self.min_nr_numa_nodes


class Worker(object):
    """
    Class for storing information about the workers to be used in scalability experiments

    A worker is a piece of hardware. Three kinds of workers can be incrementally added during an experiment:

    - core: used in scalability experiments within

      - a NUMA node
      - a cluster node

    - numa_node: used in scalability experiments within a cluster node

    - cluster_node: used in scalability experiments within a cluster partition
    """

    worker: WorkerBase

    def from_json(self, data: dict[str, typing.Any]) -> tuple[str, SizeRange]:
        worker_type = data["type"]
        size_range = SizeRange.from_json(data["size_range"])

        return worker_type, size_range

    @classmethod
    def to_json(cls, worker) -> dict[str, typing.Any]:
        return {
            "type": worker.worker.name,
            "size_range": SizeRange.to_json(worker.worker.size_range),
        }

    def __init__(
        self, data: dict[str, typing.Any], cluster: Cluster, locality_per: str
    ):
        worker_type, size_range = self.from_json(data)

        worker_type_classes: list[typing.Any] = [Core, NUMANode, ClusterNode]
        worker_class_by_worker_type = {
            worker_type_class.name: worker_type_class
            for worker_type_class in worker_type_classes
        }
        self.worker = worker_class_by_worker_type[worker_type](
            size_range, cluster, locality_per
        )

        assert 1 <= self.worker.min_nr_cluster_nodes <= self.worker.max_nr_cluster_nodes
        assert 1 <= self.worker.min_nr_numa_nodes <= self.worker.max_nr_numa_nodes
        assert 1 <= self.worker.min_nr_cores <= self.worker.max_nr_cores

        assert (
            sum(
                [
                    self.scale_over_cores,
                    self.scale_over_numa_nodes,
                    self.scale_over_cluster_nodes,
                ]
            )
            <= 1
        )

    def __str__(self) -> str:
        return "Worker(type={}, size_range={})".format(
            self.worker.name,
            self.worker.size_range,
        )

    @property
    def name(self):
        """
        Return name of worker (one of: core, numa_node, cluster_node)
        """
        return self.worker.name

    @property
    def scale_over_cluster_nodes(self) -> int:
        """
        Return whether scaling happens over cluster nodes
        """
        return self.worker.nr_cluster_nodes_range > 1

    @property
    def min_nr_cluster_nodes(self) -> int:
        return self.worker.min_nr_cluster_nodes

    @property
    def max_nr_cluster_nodes(self) -> int:
        return self.worker.max_nr_cluster_nodes

    @property
    def nr_cluster_nodes(self) -> int:
        """
        Return number of nodes to use in benchmark

        This assumes that the range in number of nodes to use is zero
        """
        return self.worker.nr_cluster_nodes

    @property
    def scale_over_numa_nodes(self) -> int:
        """
        Return whether scaling happens over NUMA nodes
        """
        return self.worker.nr_numa_nodes_range > 1

    @property
    def min_nr_numa_nodes(self) -> int:
        return self.worker.min_nr_numa_nodes

    @property
    def max_nr_numa_nodes(self) -> int:
        return self.worker.max_nr_numa_nodes

    @property
    def nr_numa_nodes(self) -> int:
        """
        Return number of NUMA nodes to use in benchmark

        This assumes that the range in number of NUMA nodes to use is zero
        """
        return self.worker.nr_numa_nodes

    @property
    def scale_over_cores(self) -> int:
        """
        Return whether scaling happens over CPU cores
        """
        return self.worker.nr_cores_range > 1

    @property
    def min_nr_cores(self) -> int:
        return self.worker.min_nr_cores

    @property
    def max_nr_cores(self) -> int:
        return self.worker.max_nr_cores

    @property
    def nr_cores(self) -> int:
        """
        Return number of cores to use in benchmark

        This assumes that the range in number of cores to use is zero
        """
        return self.worker.nr_cores

    @property
    def nr_benchmarks(self) -> int:
        """
        Return number of benchmarks to perform for benchmark
        """
        return self.worker.size_range.nr_sizes

    def nr_workers(self, benchmark_idx) -> int:
        """
        Return number of workers to use in benchmark with the index passed in
        """
        return self.worker.size_range.size(benchmark_idx)

    @property
    def max_nr_workers(self) -> int:
        """
        Return the maximum number of workers to use
        """
        return self.worker.max_nr_workers

    @property
    def nr_localities(self) -> int:
        """
        Return number of localities to use in benchmark
        """
        return self.worker.nr_localities
