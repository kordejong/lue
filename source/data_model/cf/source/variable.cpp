#include "lue/cf/variable.hpp"
#include "lue/netcdf4/type.hpp"


namespace lue::cf {

    auto Variable::kind(netcdf::Variable const& variable) -> Kind
    {
        Kind kind{Kind::regular};

        if (variable.nr_dimensions() == 1 && variable.name() == variable.dimensions()[0].name() &&
            netcdf::is_numeric(variable.type())
            // TODO(?)
            // monotonic_ordered() &&
            // nr_missing_values() == 0
        )
        {
            kind = Kind::coordinate;
        }

        return kind;
    }


    auto Variable::is_coordinate(netcdf::Variable const& variable) -> bool
    {
        return kind(variable) == Kind::coordinate;
    }


    auto Variable::is_scalar(netcdf::Variable const& variable) -> bool
    {
        return variable.nr_dimensions() == 0;
    }

}  // namespace lue::cf
