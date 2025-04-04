#pragma once
#include "lue/netcdf4.hpp"
#include <string>
#include <vector>


namespace lue::utility {

    auto describe_netcdf4_dataset(netcdf::Dataset const& dataset) -> std::vector<std::string>;

}  // namespace lue::utility
