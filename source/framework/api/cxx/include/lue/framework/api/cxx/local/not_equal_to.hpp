#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/field.hpp"


namespace lue::api {

    auto LUE_FAPI_EXPORT not_equal_to(Field const& field1, Field const& field2) -> Field;


    inline auto operator!=(lue::api::Field const& lhs, lue::api::Field const& rhs)
    {
        return lue::api::not_equal_to(lhs, rhs);
    }

}  // namespace lue::api
