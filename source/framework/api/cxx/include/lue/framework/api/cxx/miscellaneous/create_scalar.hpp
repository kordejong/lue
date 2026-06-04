#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/literal.hpp"
#include "lue/framework/api/cxx/scalar.hpp"


namespace lue::api {

    LUE_FAPI_EXPORT auto create_scalar(Literal const& value) -> Scalar;

}  // namespace lue::api
