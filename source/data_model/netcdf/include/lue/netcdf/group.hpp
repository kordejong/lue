#pragma once
#include "lue/netcdf/attribute.hpp"
#include "lue/netcdf/dimension.hpp"
#include "lue/netcdf/variable.hpp"
#include <netcdf.h>
#include <string>
#include <vector>


namespace lue::netcdf {

    class Group
    {

        public:

            Group(int group_id);

            Group(Group const& other) = delete;

            Group(Group&& other) noexcept;

            ~Group() = default;

            auto operator=(Group const& other) -> Group& = delete;

            auto operator=(Group&& other) noexcept -> Group&;

            [[nodiscard]] auto id() const -> int;

            [[nodiscard]] auto define_variable(
                std::string const& name,
                nc_type data_type,
                std::vector<Dimension> const& dimensions) const -> Variable;

            /*!
                @brief      Write a global attribute to the group
                @param      name Name of attribute
                @param      values Values of attribute
                @return     Written attribute
                @exception  std::runtime_error In case the attribute cannot be written
            */
            template<typename T>
            [[nodiscard]] auto put_attribute(std::string name, std::vector<T> const& values) -> Attribute;


            template<typename T>
            [[nodiscard]] auto put_attribute(std::string name, T const& value) -> Attribute
            {
                return put_attribute(std::move(name), std::vector<T>{value});
            }

        private:

            //! ID of the group
            int _id;
    };

}  // namespace lue::netcdf
