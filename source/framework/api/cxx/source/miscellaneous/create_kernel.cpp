#include "lue/framework/api/cxx/miscellaneous/create_kernel.hpp"
#include "lue/framework/algorithm/kernel.hpp"
#include "lue/concept.hpp"
#include "lue/framework.hpp"


namespace lue::api {

    auto create_box_kernel(Count const radius) -> BooleanKernel
    {
        return box_kernel<BooleanElement, 2>(radius, 1);
    }

}  // namespace lue::api
