#include "lue/framework/core/locality.hpp"
#include <hpx/runtime_distributed.hpp>


namespace lue {

    auto on_root_locality() -> bool
    {
        return hpx::find_here() == hpx::find_root_locality();
    }

}  // namespace lue
