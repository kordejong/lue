from ..lue_py.framework import (
    arithmetic_element_types,
    boolean_element_type,
    count_element_type,
    floating_point_element_types,
    flow_direction_element_type,
    id_element_type,
    index_element_type,
    integral_element_types,
    material_element_types,
    signed_arithmetic_element_types,
    signed_integral_element_types,
    unsigned_integral_element_types,
    zone_element_types,
)

from ..lue_py.framework_x import *
from ..lue_py import framework as lfr
from ..lue_py import framework_x as lfrx

lfrx.Progressor = lfr.Progressor
lfrx.start_hpx_runtime = lfr.start_hpx_runtime
lfrx.stop_hpx_runtime = lfr.stop_hpx_runtime
lfrx.on_root_locality = lfr.on_root_locality

__all__ = [
    "arithmetic_element_types",
    "boolean_element_type",
    "count_element_type",
    "floating_point_element_types",
    "flow_direction_element_type",
    "id_element_type",
    "index_element_type",
    "integral_element_types",
    "material_element_types",
    "signed_arithmetic_element_types",
    "signed_integral_element_types",
    "unsigned_integral_element_types",
    "zone_element_types",
]


default_hpx_runtime_configuration: list[str] = []


def runtime_scope(user_main):
    """
    Decorator to use for functions that require the HPX runtime to be started at function entry and stopped
    at function exit.

    The start and stop of the HPX runtime is executed on all localities. The function passed in is only
    called on the root locality.
    """

    def decorated_function(*args, **kwargs):
        lfrx.start_hpx_runtime(default_hpx_runtime_configuration)

        if lfrx.on_root_locality():
            user_main(*args, **kwargs)

        lfrx.stop_hpx_runtime()

    return decorated_function


import sys


class DefaultProgressor(lfrx.Progressor):
    def __init__(self):
        lfrx.Progressor.__init__(self)

    def preprocess(self, sample_nr):
        sys.stdout.write("[")
        sys.stdout.flush()

    def initialize(self):
        sys.stdout.write("[")
        sys.stdout.flush()

    def simulate(self, time_step):
        sys.stdout.write(".")
        sys.stdout.flush()

    def finalize(self):
        sys.stdout.write("]")
        sys.stdout.flush()

    def postprocess(self):
        sys.stdout.write("]")
        sys.stdout.flush()


class SilentProgressor(lfrx.Progressor):
    def __init__(self):
        lfrx.Progressor.__init__(self)
