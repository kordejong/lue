#pragma once
#include "lue/framework/api/c/kernel.h"
#include "lue/framework/api/cxx/kernel.hpp"
#include <cassert>


inline auto lue_as_cxx_boolean_kernel_ptr(LUE_BooleanKernel const* kernel) -> lue::api::BooleanKernel const*
{
    assert(kernel);
    assert(kernel->instance);

    return static_cast<lue::api::BooleanKernel const*>(kernel->instance);
}


inline auto lue_as_cxx_boolean_kernel_ptr(LUE_BooleanKernel* kernel) -> lue::api::BooleanKernel*
{
    assert(kernel);
    assert(kernel->instance);

    return static_cast<lue::api::BooleanKernel*>(kernel->instance);
}


inline auto lue_as_cxx_boolean_kernel(LUE_BooleanKernel const* kernel) -> lue::api::BooleanKernel const&
{
    return *lue_as_cxx_boolean_kernel_ptr(kernel);
}


inline auto lue_as_cxx_boolean_kernel(LUE_BooleanKernel* kernel) -> lue::api::BooleanKernel&
{
    return *lue_as_cxx_boolean_kernel_ptr(kernel);
}
