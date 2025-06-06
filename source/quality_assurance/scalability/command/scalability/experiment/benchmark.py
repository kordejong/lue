from ..core import json_to_data
from . import worker


class HPX(object):
    def __init__(self, data):
        self.from_json(data)

    def from_json(self, data):
        self.performance_counters = (
            data["performance_counters"] if "performance_counters" in data else None
        )

        if self.performance_counters:
            self.counter_interval = None

            for argument in self.performance_counters:
                if "print-counter-interval" in argument:
                    # milliseconds
                    self.counter_interval = int(argument["print-counter-interval"])

            assert self.counter_interval

    def to_json(self):
        result = {}

        if self.performance_counters:
            result["performance_counters"] = self.performance_counters

        return result


class Benchmark(object):
    def __init__(self, data, cluster):
        """
        Class for storing information about the configuration of the
        scalability experiment benchmarks

        count: How many times a benchmark needs to be repeated
        locality_per: Whether to launch a process per cluster node or
            NUMA node
        worker: Information about the workers to be used
        """
        self.from_json(data, cluster)

        if self.worker.type == "thread":
            if self.locality_per == "numa_node":
                self.nr_logical_cores_per_locality = (
                    cluster.cluster_node.package.numa_node.nr_threads
                )
                self.nr_physical_cores_per_locality = (
                    cluster.cluster_node.package.numa_node.nr_cores
                )

                assert (
                    self.worker.min_nr_cluster_nodes == 1
                ), self.worker.min_nr_cluster_nodes
                assert (
                    self.worker.max_nr_cluster_nodes == 1
                ), self.worker.max_nr_cluster_nodes
                assert self.worker.min_nr_numa_nodes == 1, self.worker.min_nr_numa_nodes
                assert self.worker.max_nr_numa_nodes == 1, self.worker.max_nr_numa_nodes
                assert self.worker.min_nr_threads >= 1, self.worker.min_nr_threads
                assert (
                    self.worker.max_nr_threads <= self.nr_physical_cores_per_locality
                ), self.worker.max_nr_threads

            elif self.locality_per == "cluster_node":
                self.nr_logical_cores_per_locality = cluster.cluster_node.nr_threads
                self.nr_physical_cores_per_locality = cluster.cluster_node.nr_cores

                assert (
                    self.worker.min_nr_cluster_nodes == 1
                ), self.worker.min_nr_cluster_nodes
                assert (
                    self.worker.max_nr_cluster_nodes == 1
                ), self.worker.max_nr_cluster_nodes
                assert (
                    self.worker.min_nr_numa_nodes == self.worker.min_nr_numa_nodes
                ), self.worker.min_nr_numa_nodes
                assert (
                    self.worker.max_nr_numa_nodes == self.worker.max_nr_numa_nodes
                ), self.worker.max_nr_numa_nodes
                assert self.worker.min_nr_threads >= 1, self.worker.min_nr_threads
                assert (
                    self.worker.max_nr_threads <= self.nr_physical_cores_per_locality
                ), self.worker.max_nr_threads

        elif self.worker.type == "numa_node":
            assert self.locality_per == "numa_node"

            self.nr_logical_cores_per_locality = (
                cluster.cluster_node.package.numa_node.nr_threads
            )
            self.nr_physical_cores_per_locality = (
                cluster.cluster_node.package.numa_node.nr_cores
            )

            assert (
                self.worker.min_nr_cluster_nodes == 1
            ), self.worker.min_nr_cluster_nodes
            assert (
                self.worker.max_nr_cluster_nodes == 1
            ), self.worker.max_nr_cluster_nodes
            assert self.worker.min_nr_numa_nodes >= 1, self.worker.min_nr_numa_nodes
            assert (
                self.worker.max_nr_numa_nodes <= cluster.cluster_node.nr_numa_nodes
            ), self.worker.min_nr_numa_nodes
            assert (
                self.worker.min_nr_threads == self.nr_physical_cores_per_locality
            ), self.worker.min_nr_threads
            assert (
                self.worker.max_nr_threads == self.nr_physical_cores_per_locality
            ), self.worker.max_nr_threads

        elif self.worker.type == "cluster_node":
            assert self.locality_per == "numa_node"

            self.nr_logical_cores_per_locality = (
                cluster.cluster_node.package.numa_node.nr_threads
            )
            self.nr_physical_cores_per_locality = (
                cluster.cluster_node.package.numa_node.nr_cores
            )

            assert (
                self.worker.min_nr_cluster_nodes >= 1
            ), self.worker.min_nr_cluster_nodes
            assert (
                self.worker.max_nr_cluster_nodes <= cluster.nr_cluster_nodes
            ), self.worker.max_nr_cluster_nodes
            assert (
                self.worker.min_nr_numa_nodes == cluster.cluster_node.nr_numa_nodes
            ), self.worker.min_nr_numa_nodes
            assert (
                self.worker.max_nr_numa_nodes == cluster.cluster_node.nr_numa_nodes
            ), self.worker.max_nr_numa_nodes
            assert (
                self.worker.min_nr_threads == self.nr_physical_cores_per_locality
            ), self.worker.min_nr_threads
            assert (
                self.worker.max_nr_threads == self.nr_physical_cores_per_locality
            ), self.nr_physical_cores_per_locality

    def __str__(self):
        return "Benchmark(count={}, worker={})".format(self.count, self.worker)

    def from_json(self, data, cluster):
        self.scenario_name = data["scenario"] if "scenario" in data else "default"
        self.count = data["count"]

        self.locality_per = data["locality_per"]
        assert self.locality_per in ["cluster_node", "numa_node"]

        self.worker = worker.Worker(data["worker"], cluster, self.locality_per)

        self.hpx = None

        if "hpx" in data:
            hpx_json = data["hpx"]

            if isinstance(hpx_json, str):
                hpx_json = json_to_data(hpx_json)

            self.hpx = HPX(hpx_json)

    def to_json(self):
        result = {
            "scenario": self.scenario_name,
            "count": self.count,
            "locality_per": self.locality_per,
            "worker": self.worker.to_json(),
        }

        if self.hpx:
            result["hpx"] = self.hpx.to_json()
            assert result["hpx"] is not None

        return result
