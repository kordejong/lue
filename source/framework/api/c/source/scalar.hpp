#pragma once
#include "lue/framework/api/c/scalar.h"
#include "lue/framework/api/cxx/scalar.hpp"
#include <cassert>


inline auto lue_as_cxx_scalar_ptr(LUE_Scalar const* scalar) -> lue::api::Scalar const*
{
    assert(scalar);
    assert(scalar->instance);

    return static_cast<lue::api::Scalar const*>(scalar->instance);
}


inline auto lue_as_cxx_scalar_ptr(LUE_Scalar* scalar) -> lue::api::Scalar*
{
    assert(scalar);
    assert(scalar->instance);

    return static_cast<lue::api::Scalar*>(scalar->instance);
}


inline auto lue_as_cxx_scalar(LUE_Scalar const* scalar) -> lue::api::Scalar const&
{
    return *lue_as_cxx_scalar_ptr(scalar);
}


inline auto lue_as_cxx_scalar(LUE_Scalar* scalar) -> lue::api::Scalar&
{
    return *lue_as_cxx_scalar_ptr(scalar);
}
