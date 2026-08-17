#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/field.hpp"


namespace lue::api {

    LUE_FAPI_EXPORT auto cell_index(Field const& condition, Index dimension_idx) -> Field;

}  // namespace lue::api
