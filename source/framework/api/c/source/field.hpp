#pragma once
#include "lue/framework/api/c/field.h"
#include "lue/framework/api/cxx/field.hpp"
#include <cassert>


inline auto lue_as_cxx_field_ptr(LUE_Field const* field) -> lue::api::Field const*
{
    assert(field);
    assert(field->instance);

    return static_cast<lue::api::Field const*>(field->instance);
}


inline auto lue_as_cxx_field_ptr(LUE_Field* field) -> lue::api::Field*
{
    assert(field);
    assert(field->instance);

    return static_cast<lue::api::Field*>(field->instance);
}


inline auto lue_as_cxx_field(LUE_Field const* field) -> lue::api::Field const&
{
    return *lue_as_cxx_field_ptr(field);
}


inline auto lue_as_cxx_field(LUE_Field* field) -> lue::api::Field&
{
    return *lue_as_cxx_field_ptr(field);
}
