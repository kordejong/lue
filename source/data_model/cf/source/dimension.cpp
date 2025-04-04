#include "lue/cf/dimension.hpp"
#include "lue/cf/variable.hpp"
#include "lue/netcdf4/group.hpp"


namespace lue::cf {

    namespace {

        auto is_coordinate_dimension(netcdf::Dimension const& dimension, std::string const& name) -> bool
        {
            bool result{false};

            netcdf::Group group{dimension.group_id()};

            for (auto const& variable : group.variables())
            {
                // TODO units
                // && variable.attribute("units") ==
                if (Variable::is_coordinate(variable) && variable.dimensions()[0] == dimension &&
                    variable.name() == name)
                {
                    result = true;
                    break;
                }
            }

            return result;
        }

    }  // Anonymous namespace


    auto Dimension::is_latitude(netcdf::Dimension const& dimension) -> bool
    {
        return is_coordinate_dimension(dimension, "lat");
    }


    auto Dimension::is_longitude(netcdf::Dimension const& dimension) -> bool
    {
        return is_coordinate_dimension(dimension, "lon");
    }


    auto Dimension::is_time(netcdf::Dimension const& dimension) -> bool
    {
        return is_coordinate_dimension(dimension, "time");
    }


    auto Dimension::is_spatiotemporal(netcdf::Dimension const& dimension) -> bool
    {
        return is_latitude(dimension) || is_longitude(dimension) || is_time(dimension);
    }

}  // namespace lue::cf
