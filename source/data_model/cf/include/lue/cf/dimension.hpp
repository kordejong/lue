#pragma once
#include "lue/netcdf4/dimension.hpp"


namespace lue::cf {

    struct Dimension
    {

            static auto is_latitude(netcdf::Dimension const& dimension) -> bool;

            static auto is_longitude(netcdf::Dimension const& dimension) -> bool;

            static auto is_time(netcdf::Dimension const& dimension) -> bool;

            static auto is_spatiotemporal(netcdf::Dimension const& dimension) -> bool;
    };

}  // namespace lue::cf
