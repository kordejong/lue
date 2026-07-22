#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/field.hpp"
#include "lue/framework/api/cxx/literal.hpp"


namespace lue::api {

    auto LUE_FAPI_EXPORT slope(Field const& elevation, Literal const& cell_size) -> Field;

}  // namespace lue::api
