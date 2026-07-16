#include "lue/framework/api/c/local_operation.h"
#include "field.hpp"
#include "lue/framework/api/cxx/local_operation.hpp"


auto lue_abs(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::abs(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_acos(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::acos(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_add(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::add(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_asin(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::asin(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_atan(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::atan(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_atan2(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::atan2(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_ceil(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::ceil(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_close_to(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::close_to(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_cos(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::cos(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_divide(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::divide(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_equal_to(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::equal_to(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_exp(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::exp(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_floor(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::floor(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_greater_than(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::greater_than(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_greater_than_equal_to(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::greater_than_equal_to(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_less_than(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::less_than(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_less_than_equal_to(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::less_than_equal_to(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_log(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::log(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_log10(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::log10(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_logical_and(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::logical_and(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_logical_exclusive_or(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::logical_exclusive_or(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_logical_inclusive_or(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::logical_inclusive_or(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_logical_not(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::logical_not(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_modulus(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::modulus(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_multiply(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::multiply(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_negate(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::negate(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_not_equal_to(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::not_equal_to(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_open_simplex_noise(LUE_Field const* x_coordinates, LUE_Field const* y_coordinates, int const seed)
    -> LUE_Field*
{
    lue::api::Field result =
        lue::api::open_simplex_noise(lue_as_cxx_field(x_coordinates), lue_as_cxx_field(y_coordinates), seed);

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_pow(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::pow(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_round(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::round(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_sin(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::sin(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_sqrt(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::sqrt(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_subtract(LUE_Field const* field1, LUE_Field const* field2) -> LUE_Field*
{
    lue::api::Field result = lue::api::subtract(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_tan(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::tan(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_trunc(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::trunc(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_valid(LUE_Field const* field) -> LUE_Field*
{
    lue::api::Field result = lue::api::valid(lue_as_cxx_field(field));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_where(LUE_Field const* field1, LUE_Field const* field2, LUE_Field const* field3) -> LUE_Field*
{
    lue::api::Field result =
        field3 ? lue::api::where(lue_as_cxx_field(field1), lue_as_cxx_field(field2), lue_as_cxx_field(field3))
               : lue::api::where(lue_as_cxx_field(field1), lue_as_cxx_field(field2));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}
