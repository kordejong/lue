#pragma once
#include "lue/concept.hpp"
#include "lue/netcdf4/error.hpp"
#include "lue/netcdf4/type_traits.hpp"
#include <netcdf.h>
#include <cassert>
#include <format>
#include <optional>
#include <string>
#include <vector>


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
                    ValueType& value) -> int
                {
                    return nc_get_att_schar(group_id, variable_id, name.c_str(), &value);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::vector<ValueType> const& values) -> int
                {
                    return nc_put_att_schar(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        std::size(values),
                        values.data());
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
                    std::uint8_t& value) -> int
                {
                    return nc_get_att_ubyte(group_id, variable_id, name.c_str(), &value);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::vector<std::uint8_t> const& values) -> int
                {
                    return nc_put_att_ubyte(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        std::size(values),
                        values.data());
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
                    std::int32_t& value) -> int
                {
                    return nc_get_att_int(group_id, variable_id, name.c_str(), &value);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::vector<std::int32_t> const& values) -> int
                {
                    return nc_put_att_int(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        std::size(values),
                        values.data());
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
                    std::uint32_t& value) -> int
                {
                    return nc_get_att_uint(group_id, variable_id, name.c_str(), &value);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::vector<std::uint32_t> const& values) -> int
                {
                    return nc_put_att_uint(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        std::size(values),
                        values.data());
                }
        };


        template<>
        struct AttributeValue<std::int64_t>
        {
                using ValueType = std::int64_t;

                static auto get(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::int64_t& value) -> int
                {
                    return nc_get_att_long(group_id, variable_id, name.c_str(), &value);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::vector<std::int64_t> const& values) -> int
                {
                    return nc_put_att_long(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        std::size(values),
                        values.data());
                }
        };


        // template<>
        // struct AttributeValue<std::uint64_t>
        // {
        //     using ValueType = std::uint64_t;
        //
        //     static auto get(int const group_id, int const variable_id, std::string const& name,
        //     std::uint64_t& value) -> int
        //     {
        //         // TODO
        //         assert(false);
        //         // return nc_get_att_ulong(group_id, variable_id, name.c_str(), &value);
        //     }

        //     static auto put(int const group_id, int const variable_id, std::string const& name,
        //     std::vector<std::uint64_t> const& values) -> int
        //     {
        //         // TODO
        //         assert(false);
        //         // return nc_put_att_ulonglong(group_id, variable_id, name.c_str(),
        //         TypeTraits<ValueType>::type_id, std::size(values), values.data());
        //     }
        // };


        template<>
        struct AttributeValue<float>
        {
                using ValueType = float;

                static auto get(
                    int const group_id, int const variable_id, std::string const& name, float& value) -> int
                {
                    return nc_get_att_float(group_id, variable_id, name.c_str(), &value);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::vector<float> const& values) -> int
                {
                    return nc_put_att_float(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        std::size(values),
                        values.data());
                }
        };


        template<>
        struct AttributeValue<double>
        {
                using ValueType = double;

                static auto get(
                    int const group_id, int const variable_id, std::string const& name, double& value) -> int
                {
                    return nc_get_att_double(group_id, variable_id, name.c_str(), &value);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::vector<double> const& values) -> int
                {
                    return nc_put_att_double(
                        group_id,
                        variable_id,
                        name.c_str(),
                        TypeTraits<ValueType>::type_id,
                        std::size(values),
                        values.data());
                }
        };

    }  // namespace detail

    class LUE_NETCDF4_EXPORT Attribute
    {

        public:

            static auto add_attribute(
                int const group_id,
                int const variable_id,
                std::string name,
                std::string const& value) -> Attribute
            {
                if (int status =
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
                int const group_id, int const variable_id, std::string name, std::vector<T> const& values)
                -> Attribute
            {
                int status{-1};

                status = detail::AttributeValue<T>::put(group_id, variable_id, name, values);

                if (status != NC_NOERR)
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
                return add_attribute(
                    group_id, variable_id, std::move(name), std::vector<T>{std::forward<T>(value)});
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

                if (int status = nc_inq_attlen(_group_id, _variable_id, _name.c_str(), &length);
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

                if (int status = nc_get_att_text(_group_id, _variable_id, _name.c_str(), value.data());
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

                int status{-1};

                T value{};

                status = detail::AttributeValue<T>::get(_group_id, _variable_id, _name, value);

                if (status != NC_NOERR)
                {
                    throw std::runtime_error(
                        std::format("Cannot obtain attribute value {}: {}", _name, error_message(status)));
                }

                return value;
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
