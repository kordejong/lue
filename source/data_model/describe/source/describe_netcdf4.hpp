#pragma once
#include "lue/netcdf4.hpp"
#include <nlohmann/json.hpp>


namespace lue::netcdf {

    void to_json(nlohmann::ordered_json& json, Dataset const& dataset);

}  // namespace lue::netcdf
