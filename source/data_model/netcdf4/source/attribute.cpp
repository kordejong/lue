#include "lue/netcdf4/attribute.hpp"
#include <array>


namespace lue::netcdf {

    auto Attribute::open(int const group_id, int const variable_id, int attribute_idx) -> Attribute
    {
        std::array<char, NC_MAX_NAME + 1> buffer{};

        if (auto const status = nc_inq_attname(group_id, variable_id, attribute_idx, buffer.data());
            status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get attribute name: {}", error_message(status)));
        }

        return {group_id, variable_id, buffer.data()};
    }


    Attribute::Attribute(int const group_id, int const variable_id, std::string name):

        _group_id{group_id},
        _variable_id{variable_id},
        _name{std::move(name)}

    {
    }


    auto Attribute::name() const -> std::string const&
    {
        return _name;
    }


    auto Attribute::type() const -> nc_type
    {
        nc_type type{0};

        if (auto const status = nc_inq_atttype(_group_id, _variable_id, _name.c_str(), &type);
            status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get attribute type: {}", error_message(status)));
        }

        return type;
    }


    auto Attribute::length() const -> std::size_t
    {
        std::size_t length{0};

        if (auto const status = nc_inq_attlen(_group_id, _variable_id, _name.c_str(), &length);
            status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get attribute length: {}", error_message(status)));
        }

        return length;
    }

}  // namespace lue::netcdf
