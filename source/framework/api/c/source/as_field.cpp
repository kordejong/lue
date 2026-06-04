#include "lue/framework/api/c/as_field.h"
#include "array.hpp"
#include "literal.hpp"
#include "scalar.hpp"
#include "lue/framework/api/c/destruct.h"
#include "lue/framework/api/cxx/as_field.hpp"
#include <cassert>


auto lue_array_as_field(LUE_Array* array) -> LUE_Field*
{
    auto* result =
        new LUE_Field{.instance = new lue::api::Field{lue::api::as_field(lue_as_cxx_array(array))}};

    lue_destruct_array(array);

    return result;
}


auto lue_literal_as_field(LUE_Literal* literal) -> LUE_Field*
{
    auto* result =
        new LUE_Field{.instance = new lue::api::Field{lue::api::as_field(lue_as_cxx_literal(literal))}};

    lue_destruct_literal(literal);

    return result;
}


auto lue_scalar_as_field(LUE_Scalar* scalar) -> LUE_Field*
{
    auto* result =
        new LUE_Field{.instance = new lue::api::Field{lue::api::as_field(lue_as_cxx_scalar(scalar))}};

    lue_destruct_scalar(scalar);

    return result;
}
