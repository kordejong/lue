#pragma once
#include "lue/netcdf4/export.hpp"
#include <string>


namespace lue::netcdf {

    LUE_NETCDF4_EXPORT auto error_message(int status) -> std::string;

}  // namespace lue::netcdf
