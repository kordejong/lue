#include "lue/framework/api/c/focal_operation.h"
#include "field.hpp"
#include "kernel.hpp"
#include "literal.hpp"
#include "lue/framework/api/cxx/focal_operation.hpp"


auto lue_aspect(LUE_Field const* elevation) -> LUE_Field*
{
    lue::api::Field result = lue::api::aspect(lue_as_cxx_field(elevation));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_convolve(LUE_Field const* field, LUE_Kernel const* kernel) -> LUE_Field*
{
    lue::api::Field result = lue::api::convolve(lue_as_cxx_field(field), lue_as_cxx_kernel(kernel));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_focal_diversity(LUE_Field const* field, LUE_Kernel const* kernel) -> LUE_Field*
{
    lue::api::Field result = lue::api::focal_diversity(lue_as_cxx_field(field), lue_as_cxx_kernel(kernel));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_focal_high_pass(LUE_Field const* field, LUE_Kernel const* kernel) -> LUE_Field*
{
    lue::api::Field result = lue::api::focal_high_pass(lue_as_cxx_field(field), lue_as_cxx_kernel(kernel));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_focal_majority(LUE_Field const* field, LUE_Kernel const* kernel) -> LUE_Field*
{
    lue::api::Field result = lue::api::focal_majority(lue_as_cxx_field(field), lue_as_cxx_kernel(kernel));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_focal_maximum(LUE_Field const* field, LUE_Kernel const* kernel) -> LUE_Field*
{
    lue::api::Field result = lue::api::focal_maximum(lue_as_cxx_field(field), lue_as_cxx_kernel(kernel));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_focal_mean(LUE_Field const* field, LUE_Kernel const* kernel) -> LUE_Field*
{
    lue::api::Field result = lue::api::focal_mean(lue_as_cxx_field(field), lue_as_cxx_kernel(kernel));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_focal_minimum(LUE_Field const* field, LUE_Kernel const* kernel) -> LUE_Field*
{
    lue::api::Field result = lue::api::focal_minimum(lue_as_cxx_field(field), lue_as_cxx_kernel(kernel));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_focal_sum(LUE_Field const* field, LUE_Kernel const* kernel) -> LUE_Field*
{
    lue::api::Field result = lue::api::focal_sum(lue_as_cxx_field(field), lue_as_cxx_kernel(kernel));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


auto lue_slope(LUE_Field const* elevation, LUE_Literal const* cell_size) -> LUE_Field*
{
    lue::api::Field result = lue::api::slope(lue_as_cxx_field(elevation), lue_as_cxx_literal(cell_size));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}
