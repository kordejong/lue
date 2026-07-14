#pragma once
#include "lue/framework/api/cxx/array.hpp"
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/scalar.hpp"


namespace lue::api {

    LUE_FAPI_EXPORT auto array_like(Array const& array, Scalar const& fill_value) -> Array;

}  // namespace lue::api
