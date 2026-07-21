#pragma once
#include "lue/framework/api/c/kernel.h"
#include "lue/framework/api/cxx/kernel.hpp"
#include <cassert>


inline auto lue_as_cxx_kernel_ptr(LUE_Kernel const* kernel) -> lue::api::Kernel const*
{
    assert(kernel);
    assert(kernel->instance);

    return static_cast<lue::api::Kernel const*>(kernel->instance);
}


inline auto lue_as_cxx_kernel_ptr(LUE_Kernel* kernel) -> lue::api::Kernel*
{
    assert(kernel);
    assert(kernel->instance);

    return static_cast<lue::api::Kernel*>(kernel->instance);
}


inline auto lue_as_cxx_kernel(LUE_Kernel const* kernel) -> lue::api::Kernel const&
{
    return *lue_as_cxx_kernel_ptr(kernel);
}


inline auto lue_as_cxx_kernel(LUE_Kernel* kernel) -> lue::api::Kernel&
{
    return *lue_as_cxx_kernel_ptr(kernel);
}
