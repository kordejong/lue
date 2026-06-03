#pragma once
#include "lue/framework/api/c/literal.h"
#include "lue/framework/api/cxx/literal.hpp"
#include <cassert>


inline auto as_cxx_literal_ptr(Literal const* literal) -> lue::api::Literal const*
{
    assert(literal);
    assert(literal->instance);

    return static_cast<lue::api::Literal const*>(literal->instance);
}


inline auto as_cxx_literal_ptr(Literal* literal) -> lue::api::Literal*
{
    assert(literal);
    assert(literal->instance);

    return static_cast<lue::api::Literal*>(literal->instance);
}


inline auto as_cxx_literal(Literal const* literal) -> lue::api::Literal const&
{
    return *as_cxx_literal_ptr(literal);
}


inline auto as_cxx_literal(Literal* literal) -> lue::api::Literal&
{
    return *as_cxx_literal_ptr(literal);
}
