#pragma once
#include "lue/concept.hpp"
#include "lue/netcdf4/error.hpp"
#include "lue/netcdf4/type_traits.hpp"
#include <netcdf.h>
#include <cassert>
#include <format>
#include <optional>
#include <string>


namespace lue::netcdf {
    namespace detail {

        template<typename T>
        struct AttributeValue
        {
        };


        template<>
        struct AttributeValue<std::int8_t>
        {
                using ValueType = std::int8_t;

                static auto get(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    ValueType* values) -> int
                {
                    return nc_get_att_schar(group_id, variable_id, name.c_str(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::size_t const nr_values,
                    ValueType const* values) -> int
                {
                    return nc_put_att_schar(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        nr_values,
                        values);
                }
        };


        template<>
        struct AttributeValue<std::uint8_t>
        {
                using ValueType = std::uint8_t;

                static auto get(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    ValueType* values) -> int
                {
                    return nc_get_att_ubyte(group_id, variable_id, name.c_str(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::size_t const nr_values,
                    ValueType const* values) -> int
                {
                    return nc_put_att_ubyte(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        nr_values,
                        values);
                }
        };


        template<>
        struct AttributeValue<std::int32_t>
        {
                using ValueType = std::int32_t;

                static auto get(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    ValueType* values) -> int
                {
                    return nc_get_att_int(group_id, variable_id, name.c_str(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::size_t const nr_values,
                    ValueType const* values) -> int
                {
                    return nc_put_att_int(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        nr_values,
                        values);
                }
        };


        template<>
        struct AttributeValue<std::uint32_t>
        {
                using ValueType = std::uint32_t;

                static auto get(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    ValueType* values) -> int
                {
                    return nc_get_att_uint(group_id, variable_id, name.c_str(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::size_t const nr_values,
                    ValueType const* values) -> int
                {
                    return nc_put_att_uint(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        nr_values,
                        values);
                }
        };


        template<>
        struct AttributeValue<std::int64_t>
        {
                using ValueType = std::int64_t;
                static_assert(std::is_same_v<ValueType, long>);

                static auto get(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    ValueType* values) -> int
                {
                    return nc_get_att_long(group_id, variable_id, name.c_str(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::size_t const nr_values,
                    ValueType const* values) -> int
                {
                    return nc_put_att_long(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        nr_values,
                        values);
                }
        };


        template<>
        struct AttributeValue<std::uint64_t>
        {
                using ValueType = std::uint64_t;

                // The NetCDF API does not provide an overload for uint64_t attribute values. It seems the
                // ulonglong version can be used (but maybe not on all platforms?).
                static_assert(std::is_same_v<ValueType, unsigned long>);
                static_assert(sizeof(ValueType) == sizeof(unsigned long long));

                static auto get(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    ValueType* values) -> int
                {
                    return nc_get_att_ulonglong(
                        group_id, variable_id, name.c_str(), (unsigned long long*)values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::size_t const nr_values,
                    ValueType const* values) -> int
                {
                    return nc_put_att_ulonglong(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        nr_values,
                        (unsigned long long const*)values);
                }
        };


        template<>
        struct AttributeValue<float>
        {
                using ValueType = float;

                static auto get(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    ValueType* values) -> int
                {
                    return nc_get_att_float(group_id, variable_id, name.c_str(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::size_t const nr_values,
                    ValueType const* values) -> int
                {
                    return nc_put_att_float(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        nr_values,
                        values);
                }
        };


        template<>
        struct AttributeValue<double>
        {
                using ValueType = double;

                static auto get(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    ValueType* values) -> int
                {
                    return nc_get_att_double(group_id, variable_id, name.c_str(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::size_t const nr_values,
                    ValueType const* values) -> int
                {
                    return nc_put_att_double(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        nr_values,
                        values);
                }
        };

    }  // namespace detail


    /*!
        @brief      Class for representing attributes
    */
    class LUE_NETCDF4_EXPORT Attribute
    {

        public:

            static auto add_attribute(
                int const group_id,
                int const variable_id,
                std::string name,
                std::string const& value) -> Attribute
            {
                if (auto const status =
                        nc_put_att_text(group_id, variable_id, name.c_str(), value.size(), value.c_str());
                    status != NC_NOERR)
                {
                    throw std::runtime_error(
                        std::format("Cannot add attribute {}: {}", name, error_message(status)));
                }

                return {group_id, variable_id, std::move(name)};
            }


            template<Arithmetic T>
            static auto add_attribute(
                int const group_id,
                int const variable_id,
                std::string name,
                std::size_t const nr_values,
                T const* values) -> Attribute
            {
                if (auto const status =
                        detail::AttributeValue<T>::put(group_id, variable_id, name, nr_values, values);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(
                        std::format("Cannot add attribute {}: {}", name, error_message(status)));
                }

                return {group_id, variable_id, std::move(name)};
            }


            template<Arithmetic T>
            static auto add_attribute(int const group_id, int const variable_id, std::string name, T&& value)
                -> Attribute
            {
                return add_attribute(group_id, variable_id, std::move(name), &value);
            }

            static auto open(int group_id, int variable_id, int attribute_idx) -> Attribute;

            Attribute(int group_id, int variable_id, std::string name);

            Attribute(Attribute const& other) = default;

            Attribute(Attribute&& other) noexcept = default;

            ~Attribute() = default;

            auto operator=(Attribute const& other) -> Attribute& = default;

            auto operator=(Attribute&& other) noexcept -> Attribute& = default;

            [[nodiscard]] auto name() const -> std::string const&;

            [[nodiscard]] auto type() const -> nc_type;

            [[nodiscard]] auto length() const -> std::size_t;


            [[nodiscard]] auto value(std::optional<std::string> default_value = {}) const -> std::string
            {
                std::size_t length{0};

                if (auto const status = nc_inq_attlen(_group_id, _variable_id, _name.c_str(), &length);
                    status != NC_NOERR)
                {
                    if (status == NC_ENOTATT && default_value)
                    {
                        return default_value.value();
                    }

                    throw std::runtime_error(
                        std::format("Cannot get attribute length: {}", error_message(status)));
                }

                std::size_t nr_bytes{this->length()};
                std::string value(nr_bytes, 'x');

                if (auto const status = nc_get_att_text(_group_id, _variable_id, _name.c_str(), value.data());
                    status != NC_NOERR)
                {
                    throw std::runtime_error(
                        std::format("Cannot obtain attribute value {}: {}", _name, error_message(status)));
                }

                return value;
            }


            template<Arithmetic T>
            auto value() const -> T
            {
                std::size_t length{this->length()};

                if (length != 1)
                {
                    throw std::runtime_error(std::format("Attribute value {} is not scalar", _name));
                }

                T value{};

                if (auto const status =
                        detail::AttributeValue<T>::get(_group_id, _variable_id, _name, &value);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(
                        std::format("Cannot obtain attribute value {}: {}", _name, error_message(status)));
                }

                return value;
            }


            template<Arithmetic T>
            void values(T* values) const
            {
                if (auto const status =
                        detail::AttributeValue<T>::get(_group_id, _variable_id, _name, values);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(
                        std::format("Cannot obtain attribute value {}: {}", _name, error_message(status)));
                }
            }

        private:

            //! ID of the group
            int _group_id;

            //! ID of the variable
            int _variable_id;

            //! Name of the attribute
            std::string _name;
    };

}  // namespace lue::netcdf
