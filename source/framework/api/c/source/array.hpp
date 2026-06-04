#pragma once
#include "lue/framework/api/c/array.h"
#include "lue/framework/api/cxx/array.hpp"
#include <cassert>


inline auto lue_as_cxx_array_ptr(LUE_Array const* array) -> lue::api::Array const*
{
    assert(array);
    assert(array->instance);

    return static_cast<lue::api::Array const*>(array->instance);
}


inline auto lue_as_cxx_array_ptr(LUE_Array* array) -> lue::api::Array*
{
    assert(array);
    assert(array->instance);

    return static_cast<lue::api::Array*>(array->instance);
}


inline auto lue_as_cxx_array(LUE_Array const* array) -> lue::api::Array const&
{
    return *lue_as_cxx_array_ptr(array);
}


inline auto as_cxx_array(LUE_Array* array) -> lue::api::Array&
{
    return *lue_as_cxx_array_ptr(array);
}
