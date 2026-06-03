#pragma once
#include "lue/framework/api/c/scalar.h"
#include "lue/framework/api/cxx/scalar.hpp"
#include <cassert>


inline auto as_cxx_scalar_ptr(Scalar const* scalar) -> lue::api::Scalar const*
{
    assert(scalar);
    assert(scalar->instance);

    return static_cast<lue::api::Scalar const*>(scalar->instance);
}


inline auto as_cxx_scalar_ptr(Scalar* scalar) -> lue::api::Scalar*
{
    assert(scalar);
    assert(scalar->instance);

    return static_cast<lue::api::Scalar*>(scalar->instance);
}


inline auto as_cxx_scalar(Scalar const* scalar) -> lue::api::Scalar const&
{
    return *as_cxx_scalar_ptr(scalar);
}


inline auto as_cxx_scalar(Scalar* scalar) -> lue::api::Scalar&
{
    return *as_cxx_scalar_ptr(scalar);
}
