#include "lue/netcdf4/attribute.hpp"


namespace lue::netcdf {

    Attribute::Attribute(int const group_id, int const variable_id, std::string name):

        _group_id{group_id},
        _variable_id{variable_id},
        _name{std::move(name)}

    {
    }


    auto Attribute::type() const -> nc_type
    {
        nc_type type{0};

        if (int status = nc_inq_atttype(_group_id, _variable_id, _name.c_str(), &type); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get attribute type: {}", error_message(status)));
        }

        return type;
    }


    auto Attribute::length() const -> std::size_t
    {
        std::size_t length{0};

        if (int status = nc_inq_attlen(_group_id, _variable_id, _name.c_str(), &length); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get attribute length: {}", error_message(status)));
        }

        return length;
    }

}  // namespace lue::netcdf
