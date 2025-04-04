#pragma once
#include "lue/netcdf4/attribute.hpp"
#include "lue/netcdf4/dimension.hpp"
#include "lue/netcdf4/variable.hpp"


namespace lue::netcdf {

    class LUE_NETCDF4_EXPORT Group
    {

        public:

            Group(int group_id);

            Group(Group const& other) = default;

            Group(Group&& other) noexcept;

            virtual ~Group() = default;

            auto operator=(Group const& other) -> Group& = default;

            auto operator=(Group&& other) noexcept -> Group&;

            [[nodiscard]] auto id() const -> int;

            [[nodiscard]] auto name() const -> std::string;

            [[nodiscard]] auto full_name() const -> std::string;

            [[nodiscard]] auto add_dimension(std::string const& name, size_t length) const -> Dimension;

            [[nodiscard]] auto has_dimension(std::string const& name) const -> bool;

            [[nodiscard]] auto dimension(std::string const& name) const -> Dimension;

            [[nodiscard]] auto nr_dimensions() const -> int;

            [[nodiscard]] auto dimensions() const -> std::vector<Dimension>;

            [[nodiscard]] auto add_variable(
                std::string const& name,
                nc_type data_type,
                std::vector<Dimension> const& dimensions = {}) const -> Variable;

            [[nodiscard]] auto has_variable(std::string const& name) const -> bool;

            [[nodiscard]] auto variable(std::string const& name) const -> Variable;

            [[nodiscard]] auto variables() const -> std::vector<Variable>;

            /*!
                @brief      Write a global attribute to the group
                @param      name Name of attribute
                @param      values Values of attribute
                @return     Written attribute
                @exception  std::runtime_error In case the attribute cannot be written
            */
            template<typename T>
            auto add_attribute(std::string name, std::vector<T> const& values) -> Attribute
            {
                return Attribute::add_attribute(_id, NC_GLOBAL, std::move(name), values);
            }


            template<typename T>
            auto add_attribute(std::string name, T&& value) -> Attribute
            {
                return Attribute::add_attribute(_id, NC_GLOBAL, std::move(name), std::forward<T>(value));
            }

            [[nodiscard]] auto has_attribute(std::string const& name) const -> bool;

            [[nodiscard]] auto attribute(std::string name) const -> Attribute;

            [[nodiscard]] auto attributes() const -> std::vector<Attribute>;

            [[nodiscard]] auto parent_group() const -> Group;

            [[nodiscard]] auto add_child_group(std::string const& name) const -> Group;

            [[nodiscard]] auto has_child_group(std::string const& name) const -> bool;

            [[nodiscard]] auto child_group(std::string const& name) const -> Group;

            [[nodiscard]] auto child_groups() const -> std::vector<Group>;

        protected:

            auto reset_id() -> int;

            [[nodiscard]] auto id_is_valid() const -> bool;

        private:

            [[nodiscard]] auto name_length() const -> std::size_t;

            //! ID of the group
            int _id;
    };

}  // namespace lue::netcdf
