#include "lue/cf/dimension.hpp"
#include "lue/cf/variable.hpp"
#include "lue/netcdf4/group.hpp"


namespace lue::cf {

    Dimension::Dimension(netcdf::Dimension const& dimension):

        netcdf::Dimension{dimension}

    {
    }


    auto Dimension::is_coordinate_dimension(std::string const& name) const -> bool
    {
        bool result{false};

        netcdf::Group group{group_id()};

        for (auto const& netcdf_variable : group.variables())
        {
            Variable const variable{netcdf_variable};

            // TODO units
            // && variable.attribute("units") ==
            if (variable.kind() == Variable::Kind::coordinate && variable.dimensions()[0] == *this &&
                variable.name() == name)
            {
                result = true;
                break;
            }
        }

        return result;
    }


    auto Dimension::kind() const -> Kind
    {
        Kind kind{Kind::unknown};

        if (is_coordinate_dimension("lat"))
        {
            kind = Kind::latitude;
        }
        else if (is_coordinate_dimension("lon"))
        {
            kind = Kind::longitude;
        }
        else if (is_coordinate_dimension("time"))
        {
            kind = Kind::time;
        }

        return kind;
    }


    auto Dimension::is_spatiotemporal() const -> bool
    {
        auto const kind{this->kind()};

        return kind == Kind::latitude || kind == Kind::longitude || kind == Kind::time;
    }

}  // namespace lue::cf
