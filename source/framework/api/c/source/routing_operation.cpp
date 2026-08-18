#include "lue/framework/api/c/routing_operation.h"
#include "field.hpp"
#include "lue/framework/api/cxx/routing_operation.hpp"


auto lue_d8_flow_direction(LUE_Field const* elevation) -> LUE_Field*
{
    lue::api::Field flow_direction = lue::api::d8_flow_direction(lue_as_cxx_field(elevation));

    return new LUE_Field{.instance = new lue::api::Field{std::move(flow_direction)}};
}
