#pragma once
#include "lue/netcdf4/error.hpp"
#include <fmt/format.h>
#include <netcdf.h>
#include <string>
#include <vector>


namespace lue::netcdf {

    namespace detail {

        template<typename T>
        struct AttributePutter
        {
        };


        template<>
        struct AttributePutter<int>
        {
                static auto put_att(
                    int const group_id,
                    int const variable_id,
                    std::string const& name,
                    std::vector<int> const& values) -> int
                {
                    return nc_put_att_int(
                        group_id, variable_id, name.c_str(), NC_INT, std::size(values), values.data());
                }
        };

    }  // namespace detail

    class Attribute
    {

        public:

            template<typename T>
            static auto put_attribute(
                int const group_id, int const variable_id, std::string name, std::vector<T> const& values)
                -> Attribute
            {
                if (int status = detail::AttributePutter<T>::put_att(group_id, variable_id, name, values);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(
                        fmt::format("Cannot put attribute {}: {}", name, error_message(status)));
                }

                return {group_id, variable_id, std::move(name)};
            }

            template<typename T>
            static auto put_attribute(int const group_id, int const variable_id, std::string name, T&& value)
                -> Attribute
            {
                return put_attribute(
                    group_id, variable_id, std::move(name), std::vector<T>{std::forward<T>(value)});
            }

            Attribute(int group_id, int variable_id, std::string name);

            Attribute(Attribute const& other) = delete;

            Attribute(Attribute&& other) noexcept;

            ~Attribute() = default;

            auto operator=(Attribute const& other) -> Attribute& = delete;

            auto operator=(Attribute&& other) noexcept -> Attribute&;

        private:

            //! ID of the group
            int _group_id;

            //! ID of the variable
            int _variable_id;

            //! Name of the attribute
            std::string _name;
    };

}  // namespace lue::netcdf
