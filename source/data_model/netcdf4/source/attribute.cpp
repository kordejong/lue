#include "lue/netcdf4/attribute.hpp"
#include <array>


namespace lue::netcdf {

    /*!
        @brief      Open an attribute
        @param      group_id ID of variable's group
        @param      variable_id ID of variable
        @param      attribute_idx Index of attribute
        @return     Attribute instance representing the added attribute
        @exception  std::runtime_error In case the attribute cannot be opened
    */
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


    /*!
        @brief      Construct an instance representing a variable's attribute
        @param      group_id ID of variable's group
        @param      variable_id ID of variable
        @param      name Name of attribute to add
    */
    Attribute::Attribute(int const group_id, int const variable_id, std::string name):

        _group_id{group_id},
        _variable_id{variable_id},
        _name{std::move(name)}

    {
    }


    /*!
        @brief      Return the name of the attribute
    */
    auto Attribute::name() const -> std::string const&
    {
        return _name;
    }


    /*!
        @brief      Return the type of the attribute's value
        @exception  std::runtime_error In case the type cannot be obtained
    */
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


    /*!
        @brief      Return the length the attribute's value
        @exception  std::runtime_error In case the length cannot be obtained

        The length is one in case of a scalar value.
    */
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
