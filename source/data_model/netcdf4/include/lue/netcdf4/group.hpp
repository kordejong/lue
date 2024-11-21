#pragma once
#include "lue/netcdf4/attribute.hpp"
#include "lue/netcdf4/dimension.hpp"
#include "lue/netcdf4/variable.hpp"


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
            [[nodiscard]] auto put_attribute(std::string name, std::vector<T> const& values) -> Attribute
            {
                return Attribute::put_attribute(_id, NC_GLOBAL, std::move(name), values);
            }


            template<typename T>
            [[nodiscard]] auto put_attribute(std::string name, T&& value) -> Attribute
            {
                return Attribute::put_attribute(_id, NC_GLOBAL, std::move(name), std::forward<T>(value));
            }

        private:

            //! ID of the group
            int _id;
    };

}  // namespace lue::netcdf
