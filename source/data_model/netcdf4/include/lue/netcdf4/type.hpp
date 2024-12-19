#pragma once
#include <netcdf.h>


namespace lue::netcdf {

    auto is_numeric(nc_type type_id) -> bool;

    auto as_string(nc_type type_id) -> char const*;

}  // namespace lue::netcdf
