#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/field.hpp"

namespace lue::api {

    auto LUE_FAPI_EXPORT as_state(Field const& field) -> hpx::shared_future<void>;

}  // namespace lue::api
