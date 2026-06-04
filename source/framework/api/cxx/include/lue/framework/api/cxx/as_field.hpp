#pragma once
#include "lue/framework/api/cxx/array.hpp"
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/field.hpp"
#include "lue/framework/api/cxx/literal.hpp"
#include "lue/framework/api/cxx/scalar.hpp"

namespace lue::api {

    auto LUE_FAPI_EXPORT as_field(Array& array) -> Field;

    auto LUE_FAPI_EXPORT as_field(Literal& literal) -> Field;

    auto LUE_FAPI_EXPORT as_field(Scalar& scalar) -> Field;

}  // namespace lue::api
