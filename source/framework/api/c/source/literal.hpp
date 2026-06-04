#pragma once
#include "lue/framework/api/c/literal.h"
#include "lue/framework/api/cxx/literal.hpp"
#include <cassert>


inline auto lue_as_cxx_literal_ptr(LUE_Literal const* literal) -> lue::api::Literal const*
{
    assert(literal);
    assert(literal->instance);

    return static_cast<lue::api::Literal const*>(literal->instance);
}


inline auto lue_as_cxx_literal_ptr(LUE_Literal* literal) -> lue::api::Literal*
{
    assert(literal);
    assert(literal->instance);

    return static_cast<lue::api::Literal*>(literal->instance);
}


inline auto lue_as_cxx_literal(LUE_Literal const* literal) -> lue::api::Literal const&
{
    return *lue_as_cxx_literal_ptr(literal);
}


inline auto lue_as_cxx_literal(LUE_Literal* literal) -> lue::api::Literal&
{
    return *lue_as_cxx_literal_ptr(literal);
}
