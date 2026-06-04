#include "lue/framework/api/c/runtime.h"
#include <hpx/hpx_init.hpp>


auto lue_initialize_runtime(int (*entry_point)(int, char**), int argc, char** argv) -> int
{
    // Initialize HPX, run entry point as the first HPX thread, and wait for hpx::finalize being called
    return hpx::init(entry_point, argc, argv);
}


auto lue_finalize_runtime() -> int
{
    return hpx::finalize();
}
