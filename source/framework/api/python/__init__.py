from ..lue_py import framework_x as lfrx
from ..lue_py.framework_x import *


default_hpx_runtime_configuration = [
    # Make sure hpx_main is always executed
    "hpx.run_hpx_main!=1",
    # Allow for unknown command line options
    "hpx.commandline.allow_unknown!=1",
    # Disable HPX' short options
    "hpx.commandline.aliasing!=0",
    # Don't print diagnostics during forced terminate
    "hpx.diagnostics_on_terminate!=0",
    # Make AGAS clean up resources faster than by default. This decreases the amount of
    # memory used.
    "hpx.agas.max_pending_refcnt_requests!=50",
    # Got an HPX error when processing a large dataset:
    #     mmap() failed to allocate thread stack due to
    #     insufficient resources, increase
    #     /proc/sys/vm/max_map_count or add
    #     -Ihpx.stacks.use_guard_pages=0 to the command line
    # Therefore:
    "hpx.stacks.use_guard_pages!=0",
]


def runtime_scope(user_main):
    """
    Decorator to use for functions that require the HPX runtime to be started at function entry
    and stopped at function exit.

    The start and stop of the HPX runtime is executed on all localities. The function passed
    in is only called on the root locality.
    """

    def decorated_function(*args, **kwargs):
        lfrx.start_hpx_runtime(default_hpx_runtime_configuration)

        if lfrx.on_root_locality():
            user_main(*args, **kwargs)

        lfrx.stop_hpx_runtime()

    return decorated_function
