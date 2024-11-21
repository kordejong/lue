#include "lue/netcdf4/attribute.hpp"


namespace lue::netcdf {

    Attribute::Attribute(int const group_id, int const variable_id, std::string name):

        _group_id{group_id},
        _variable_id{variable_id},
        _name{std::move(name)}

    {
    }

}  // namespace lue::netcdf
