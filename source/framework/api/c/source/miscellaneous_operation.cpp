#include "lue/framework/api/c/miscellaneous_operation.h"
#include "field.hpp"
#include "literal.hpp"
#include "lue/framework/api/cxx/miscellaneous_operation.hpp"


auto create_array(Rank const rank, Count const* array_shape_p, Field const* fill_value) -> Field*
{
    assert(rank == 2);

    lue::Shape<lue::Count, 2> array_shape{};
    std::copy(array_shape_p, array_shape_p + rank, array_shape.data());

    lue::api::Field result = lue::api::create_array(array_shape, as_cxx_field(fill_value));

    return new Field{.instance = new lue::api::Field{std::move(result)}};
}


auto create_literal_float32(float const value) -> Field*
{
    lue::api::Field result{value};

    return new Field{.instance = new lue::api::Field{std::move(result)}};
}


auto create_literal_float64(double const value) -> Field*
{
    lue::api::Field result{value};

    return new Field{.instance = new lue::api::Field{std::move(result)}};
}


auto create_scalar(Literal* fill_value) -> Field*
{
    lue::api::Field result = lue::api::create_scalar(as_cxx_literal(fill_value));

    return new Field{.instance = new lue::api::Field{std::move(result)}};
}
