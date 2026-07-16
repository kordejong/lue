#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/field.hpp"


namespace lue::api {

    LUE_FAPI_EXPORT auto cast(Field const& field, ElementType element_type) -> Field;

}  // namespace lue::api
