#include "lue/cf/variable.hpp"
#include "lue/netcdf4/type.hpp"


namespace lue::cf {

    // auto Variable::is_coordinate(netcdf::Variable const& variable) -> bool
    // {
    //     return kind(variable) == Kind::coordinate;
    // }


    // auto Variable::is_scalar(netcdf::Variable const& variable) -> bool
    // {
    //     return variable.nr_dimensions() == 0;
    // }


    Variable::Variable(netcdf::Variable const& variable):

        netcdf::Variable{variable}

    {
    }


    auto Variable::standard_name() const -> std::string
    {
        return attribute("standard_name").value("");
    }


    auto Variable::long_name() const -> std::string
    {
        return attribute("long_name").value("");
    }


    auto Variable::units() const -> std::string
    {
        // TODO Required for all variables that represent dimensional quantities, except for (some) boundary
        //      variables
        // TODO Strings recognized by udunits package, with some exceptions

        return attribute("units").value();
    }


    auto Variable::kind() const -> Kind
    {
        Kind kind{Kind::regular};

        if (nr_dimensions() == 1 && name() == dimensions()[0].name() && netcdf::is_numeric(type())
            // TODO(?)
            // monotonic_ordered() &&
            // nr_missing_values() == 0
        )
        {
            kind = Kind::coordinate;
        }

        return kind;
    }

}  // namespace lue::cf
