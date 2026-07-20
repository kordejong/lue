#include "lue/framework/api/c/focal_operation.h"
#include "field.hpp"
#include "kernel.hpp"
#include "lue/framework/api/cxx/focal_operation.hpp"


auto lue_focal_mean(LUE_Field const* field, LUE_BooleanKernel const* kernel) -> LUE_Field*
{
    lue::api::Field result = lue::api::focal_mean(lue_as_cxx_field(field), lue_as_cxx_boolean_kernel(kernel));

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}
