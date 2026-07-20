#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/kernel.hpp"


namespace lue::api {

    LUE_FAPI_EXPORT auto create_box_kernel(Count radius) -> BooleanKernel;

}  // namespace lue::api
