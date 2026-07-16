#pragma once
#include "lue/framework/api/c/future.h"
#include "lue/framework/api/cxx/future.hpp"
#include <cassert>


inline auto lue_as_cxx_future_ptr(LUE_Future const* future) -> lue::api::Future const*
{
    assert(future);
    assert(future->instance);

    return static_cast<lue::api::Future const*>(future->instance);
}


inline auto lue_as_cxx_future_ptr(LUE_Future* future) -> lue::api::Future*
{
    assert(future);
    assert(future->instance);

    return static_cast<lue::api::Future*>(future->instance);
}


inline auto lue_as_cxx_future(LUE_Future const* future) -> lue::api::Future const&
{
    return *lue_as_cxx_future_ptr(future);
}


inline auto lue_as_cxx_future(LUE_Future* future) -> lue::api::Future&
{
    return *lue_as_cxx_future_ptr(future);
}
