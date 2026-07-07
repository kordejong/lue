#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/field.hpp"


namespace lue::api {

    LUE_FAPI_EXPORT auto array_like(Field const& array, Field const& fill_value) -> Field;

}  // namespace lue::api
