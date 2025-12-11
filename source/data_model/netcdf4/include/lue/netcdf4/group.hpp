#pragma once
#include "lue/netcdf4/attribute.hpp"
#include "lue/netcdf4/dimension.hpp"
#include "lue/netcdf4/variable.hpp"


namespace lue::netcdf4 {

    /*!
        @brief      Class for representing groups

        Instances can be copied and moved.
    */
    class LUE_NETCDF4_EXPORT Group
    {

        public:

            Group(int group_id);

            Group(Group const& other) = default;

            Group(Group&& other) noexcept;

            virtual ~Group() = default;

            auto operator=(Group const& other) -> Group& = default;

            auto operator=(Group&& other) noexcept -> Group&;

            auto id() const -> int;

            auto name() const -> std::string;

            auto full_name() const -> std::string;

            auto add_dimension(std::string const& name, size_t length) const -> Dimension;

            auto add_dimension(std::string const& name) const -> Dimension;

            auto has_dimension(std::string const& name) const -> bool;

            auto dimension(std::string const& name) const -> Dimension;

            auto nr_dimensions() const -> int;

            auto dimensions() const -> std::vector<Dimension>;

            auto add_variable(
                std::string const& name, nc_type data_type, std::vector<Dimension> const& dimensions = {})
                -> Variable;

            auto has_variable(std::string const& name) const -> bool;

            auto variable(std::string const& name) const -> Variable;

            auto variables() const -> std::vector<Variable>;

            auto add_attribute(std::string name, std::string const& value) -> Attribute;


            /*!
                @brief      Add attribute with name @a name and scalar value @a value
                @tparam     T Type of the attribute value
                @return     Instance representing the attribute
            */
            template<Arithmetic T>
            auto add_attribute(std::string name, T const value) -> Attribute
            {
                return Attribute::add_attribute(_id, NC_GLOBAL, std::move(name), value);
            }


            /*!
                @brief      Add attribute with name @a name and array of @a values
                @tparam     T Element type of the values
                @return     Instance representing the attribute
            */
            template<Arithmetic T>
            auto add_attribute(std::string name, std::size_t const nr_values, T const* values) -> Attribute
            {
                return Attribute::add_attribute(_id, NC_GLOBAL, std::move(name), nr_values, values);
            }


            auto has_attribute(std::string const& name) const -> bool;

            auto attribute(std::string name) const -> Attribute;

            auto attributes() const -> std::vector<Attribute>;

            auto parent_group() const -> Group;

            auto add_child_group(std::string const& name) const -> Group;

            auto has_child_group(std::string const& name) const -> bool;

            auto child_group(std::string const& name) const -> Group;

            auto child_groups() const -> std::vector<Group>;

        protected:

            auto reset_id() -> int;

            auto id_is_valid() const -> bool;

        private:

            auto name_length() const -> std::size_t;

            //! ID of the group
            int _id;
    };

}  // namespace lue::netcdf4
