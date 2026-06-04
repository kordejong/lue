#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/field.hpp"
#include "lue/framework/api/cxx/scalar.hpp"


namespace lue::api {

    LUE_FAPI_EXPORT auto create_array(Shape<Count, 2> const& array_shape, Scalar const& fill_value) -> Field;

    LUE_FAPI_EXPORT auto create_array(
        Shape<Count, 2> const& array_shape, Shape<Count, 2> const& partition_shape, Scalar const& fill_value)
        -> Field;

}  // namespace lue::api
