#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/field.hpp"


namespace lue::api {

    auto LUE_FAPI_EXPORT logical_not(Field const& field) -> Field;


    inline auto operator!(lue::api::Field const& argument)
    {
        return lue::api::logical_not(argument);
    }

}  // namespace lue::api
