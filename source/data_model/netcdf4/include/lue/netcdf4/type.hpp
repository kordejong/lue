#pragma once
#include "lue/netcdf4/export.hpp"
#include <netcdf.h>


namespace lue::netcdf4 {

    LUE_NETCDF4_EXPORT auto is_numeric(nc_type type_id) -> bool;

    LUE_NETCDF4_EXPORT auto as_string(nc_type type_id) -> char const*;

}  // namespace lue::netcdf4
