#include "lue/netcdf/attribute.hpp"
#include "lue/netcdf/error.hpp"
#include <format>


namespace lue::netcdf {

    Attribute::Attribute(int const group_id, std::string name):

        _group_id{group_id},
        _variable_id{NC_GLOBAL},
        _name{std::move(name)}

    {
    }

}  // namespace lue::netcdf
