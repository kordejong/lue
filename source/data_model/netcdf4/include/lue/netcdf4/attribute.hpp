#pragma once
#include "lue/concept.hpp"
#include "lue/netcdf4/error.hpp"
#include <netcdf.h>
#include <cassert>
#include <cstdint>
#include <format>
#include <optional>
#include <string>
#include <vector>


namespace lue::netcdf {

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

                if constexpr (std::is_same_v<T, std::int32_t>)
                {
                    status = nc_put_att_int(
                        group_id, variable_id, name.c_str(), NC_INT, std::size(values), values.data());
                }
                else
                {
                    assert(false);
                }

                // TODO Expand for all T

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


            Attribute(int group_id, int variable_id, std::string name);

            Attribute(Attribute const& other) = default;

            Attribute(Attribute&& other) noexcept = default;

            ~Attribute() = default;

            auto operator=(Attribute const& other) -> Attribute& = default;

            auto operator=(Attribute&& other) noexcept -> Attribute& = default;

            [[nodiscard]] auto type() const -> nc_type;


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

                if constexpr (std::is_same_v<T, std::int32_t>)
                {
                    status = nc_get_att_int(_group_id, _variable_id, _name.c_str(), &value);
                }
                else
                {
                    assert(false);
                }

                // TODO Expand for all T

                if (status != NC_NOERR)
                {
                    throw std::runtime_error(
                        std::format("Cannot obtain attribute value {}: {}", _name, error_message(status)));
                }

                return value;
            }

        private:

            [[nodiscard]] auto length() const -> std::size_t;

            //! ID of the group
            int _group_id;

            //! ID of the variable
            int _variable_id;

            //! Name of the attribute
            std::string _name;
    };

}  // namespace lue::netcdf
