#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/field.hpp"
#include "lue/framework/api/cxx/kernel.hpp"


namespace lue::api {

    auto LUE_FAPI_EXPORT aspect(Field const& elevation) -> Field;

}  // namespace lue::api
