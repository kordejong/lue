#pragma once
#include "lue/netcdf4/variable.hpp"


namespace lue::cf {

    struct Variable
    {

            enum class Kind {
                coordinate,
                regular,
            };

            static auto kind(netcdf::Variable const& variable) -> Kind;

            static auto is_coordinate(netcdf::Variable const& variable) -> bool;

            static auto is_scalar(netcdf::Variable const& variable) -> bool;
    };

}  // namespace lue::cf
