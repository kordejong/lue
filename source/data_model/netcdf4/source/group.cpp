#include "lue/netcdf4/group.hpp"
#include "lue/netcdf4/error.hpp"
#include <ranges>


namespace lue::netcdf4 {

    /*!
        @brief      Construct instance given @a group_id
    */
    Group::Group(int const group_id):

        _id{group_id}

    {
    }


    /*!
        @brief      Move construct an instance
        @param      other Instance whose value is not needed anymore. Afterwards, this instance will not
                    contain a valid group id anymore. This ID should not be passed to the NetCDF API.
    */
    Group::Group(Group&& other) noexcept:

        _id{other.reset_id()}

    {
        assert(!other.id_is_valid());
    }


    /*!
        @brief      Move assign an instance
        @param      other Instance whose value is not needed anymore. Afterwards, this instance will not
                    contain a valid group id anymore. This ID should not be passed to the NetCDF API.
    */
    auto Group::operator=(Group&& other) noexcept -> Group&
    {
        _id = other.reset_id();

        return *this;
    }


    /*!
        @brief      Return layered group ID
    */
    auto Group::id() const -> int
    {
        return _id;
    }


    /*!
        @brief      Reset the layered group ID

        The result of calling this function is that the layered group ID is not valid anymore. Only valid
        identifiers can be passed into NetCDF API functions.
    */
    auto Group::reset_id() -> int
    {
        auto group_id = _id;

        if (id_is_valid())
        {
            _id = -1;
        }

        assert(!id_is_valid());

        return group_id;
    }


    /*!
        @brief      Return whether the layered group ID is valid

        Only valid identifiers can be passed into NetCDF API functions.
    */
    auto Group::id_is_valid() const -> bool
    {
        return _id >= 0;
    }


    /*!
        @brief      Return the length of the name of the group, in number of bytes
        @exception  std::runtime_error In case the length cannot be obtained
    */
    auto Group::name_length() const -> std::size_t
    {
        std::size_t nr_bytes{0};

        if (auto const status = nc_inq_grpname_len(_id, &nr_bytes); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get group name length: {}", error_message(status)));
        }

        return nr_bytes;
    }


    /*!
        @brief      Return the name of the group
        @exception  std::runtime_error In case the name cannot be obtained
    */
    auto Group::name() const -> std::string
    {
        std::size_t nr_bytes{name_length()};
        std::string name(nr_bytes, 'x');

        if (auto const status = nc_inq_grpname(_id, name.data()); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get group name: {}", error_message(status)));
        }

        return name;
    }


    /*!
        @brief      Return the full name of the group
        @exception  std::runtime_error In case the name cannot be obtained
    */
    auto Group::full_name() const -> std::string
    {
        std::size_t nr_bytes{name_length()};
        std::string name(nr_bytes, 'x');

        if (auto const status = nc_inq_grpname_full(_id, nullptr, name.data()); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get full group name: {}", error_message(status)));
        }

        return name;
    }


    /*!
        @brief      Add a dimension with name @a name and length @a length
        @exception  std::runtime_error In case the dimension cannot be added
    */
    auto Group::add_dimension(std::string const& name, size_t const length) const -> Dimension
    {
        int dimension_id{};

        if (auto const status = nc_def_dim(_id, name.c_str(), length, &dimension_id); status != NC_NOERR)
        {
            throw std::runtime_error(
                std::format("Cannot define dimension {}: {}", name, error_message(status)));
        }

        return {_id, dimension_id};
    }


    /*!
        @brief      Return whether the group contains a dimension with name @a name
    */
    auto Group::has_dimension(std::string const& name) const -> bool
    {
        int dimension_id{};

        return nc_inq_dimid(_id, name.c_str(), &dimension_id) == NC_NOERR;
    }


    /*!
        @brief      Return an instance representing the dimension with name @a name
        @exception  std::runtime_error In case the dimension cannot be obtained
    */
    auto Group::dimension(std::string const& name) const -> Dimension
    {
        int dimension_id{};

        if (auto const status = nc_inq_dimid(_id, name.c_str(), &dimension_id); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get dimension {}: {}", name, error_message(status)));
        }

        return {_id, dimension_id};
    }


    /*!
        @brief      Return the number of dimensions
        @exception  std::runtime_error In case the number of dimensions cannot be obtained
    */
    auto Group::nr_dimensions() const -> int
    {
        int nr_dimensions{0};
        int const include_parents{0};

        if (auto const status = nc_inq_dimids(_id, &nr_dimensions, nullptr, include_parents);
            status != NC_NOERR)
        {
            throw std::runtime_error(
                std::format("Cannot get number of dimensions: {}", error_message(status)));
        }

        return nr_dimensions;
    }


    /*!
        @brief      Return the collection of dimensions
        @exception  std::runtime_error In case the dimensions cannot be obtained
    */
    auto Group::dimensions() const -> std::vector<Dimension>
    {
        int nr_dimensions{this->nr_dimensions()};
        std::vector<int> dimension_ids(nr_dimensions);
        int const include_parents{0};

        if (auto const status = nc_inq_dimids(_id, nullptr, dimension_ids.data(), include_parents);
            status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get dimension IDs: {}", error_message(status)));
        }

        // TODO C++23
        // auto dimensions = dimension_ids
        //     | std::views::transform(...)
        //     | std::ranges::to<std::vector>();

        auto dimensions_view =
            dimension_ids | std::views::transform([group_id = _id](auto const dimension_id)
                                                  { return Dimension{group_id, dimension_id}; });

        std::vector<Dimension> dimensions(dimensions_view.begin(), dimensions_view.end());

        return dimensions;
    }


    /*!
        @brief      Add an attribute with name @a name and value @a value
        @return     Instance representing the attribute
    */
    auto Group::add_attribute(std::string name, std::string const& value) -> Attribute
    {
        return Attribute::add_attribute(_id, NC_GLOBAL, std::move(name), value);
    }


    /*!
        @brief      Return whether the group contains an attribute with name @ name
    */
    auto Group::has_attribute(std::string const& name) const -> bool
    {
        int attribute_id{};

        return nc_inq_attid(_id, NC_GLOBAL, name.c_str(), &attribute_id) == NC_NOERR;
    }


    /*!
        @brief      Return the attribute with name @a name
        @exception  std::runtime_error In case the attribute cannot be obtained
    */
    auto Group::attribute(std::string name) const -> Attribute
    {
        if (!has_attribute(name))
        {
            throw std::runtime_error(std::format("Attribute {} is not available", name));
        }

        return {_id, NC_GLOBAL, std::move(name)};
    }


    /*!
        @brief      Return the collection of attributes
        @exception  std::runtime_error In case the attributes cannot be obtained
    */
    auto Group::attributes() const -> std::vector<Attribute>
    {
        int nr_attributes{0};

        if (auto const status = nc_inq_natts(_id, &nr_attributes); status != NC_NOERR)
        {
            throw std::runtime_error(
                std::format("Cannot get number of group attributes: {}", error_message(status)));
        }

        std::vector<Attribute> attributes;
        attributes.reserve(nr_attributes);

        for (int attribute_id = 0; attribute_id < nr_attributes; ++attribute_id)
        {
            attributes.push_back(Attribute::open(_id, NC_GLOBAL, attribute_id));
        }

        return attributes;
    }


    /*!
        @brief      Add variable
        @param      name Variable name
        @param      data_type Data type
        @param      dimensions Variable dimensions
        @return     Instance representing the variable
        @exception  std::runtime_error In case the variable cannot be added

        A default fill value will be set (as per NetCDF convention), which is dependent on the data type.
    */
    auto Group::add_variable(
        std::string const& name,
        nc_type const data_type,
        std::vector<Dimension> const& dimensions) -> Variable
    {
        int variable_id{};

        // TODO C++23
        // auto dimension_ids = dimensions
        //     | std::views::transform(...)
        //     | std::ranges::to<std::vector>();

        auto dimension_ids_view =
            dimensions | std::views::transform([](auto const& dimension) { return dimension.id(); });

        std::vector<int> dimension_ids(dimension_ids_view.begin(), dimension_ids_view.end());

        if (auto const status = nc_def_var(
                _id,
                name.c_str(),
                data_type,
                static_cast<int>(dimension_ids.size()),
                dimension_ids.data(),
                &variable_id);
            status != NC_NOERR)
        {
            throw std::runtime_error(
                std::format("Cannot define variable {}: {}", name, error_message(status)));
        }

        Variable variable{_id, variable_id};

        // If this fails, update the doc
        assert(variable.has_fill_value());

        return variable;
    }


    /*!
        @brief      Return whether the group contains a variable with name @a name
    */
    auto Group::has_variable(std::string const& name) const -> bool
    {
        int variable_id{};

        return nc_inq_varid(_id, name.c_str(), &variable_id) == NC_NOERR;
    }


    /*!
        @brief      Return the variable with name @a name
        @exception  std::runtime_error In case the variable cannot be obtained
    */
    auto Group::variable(std::string const& name) const -> Variable
    {
        int variable_id{};

        if (auto const status = nc_inq_varid(_id, name.c_str(), &variable_id); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get variable {}: {}", name, error_message(status)));
        }

        return {_id, variable_id};
    }


    /*!
        @brief      Return collection of variables
        @exception  std::runtime_error In case the variables cannot be obtained
    */
    auto Group::variables() const -> std::vector<Variable>
    {
        int nr_variables{0};

        if (auto const status = nc_inq_varids(_id, &nr_variables, nullptr); status != NC_NOERR)
        {
            throw std::runtime_error(
                std::format("Cannot get number of variables: {}", error_message(status)));
        }

        std::vector<int> variable_ids(nr_variables);

        if (auto const status = nc_inq_varids(_id, nullptr, variable_ids.data()); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get variable IDs: {}", error_message(status)));
        }

        // TODO C++23
        // auto variables = variable_ids
        //     | std::views::transform(...)
        //     | std::ranges::to<std::vector>();

        auto variables_view =
            variable_ids | std::views::transform([group_id = _id](auto const variable_id)
                                                 { return Variable{group_id, variable_id}; });

        std::vector<Variable> variables(variables_view.begin(), variables_view.end());

        return variables;
    }


    /*!
        @brief      Return an instance representing the parent group
        @exception  std::runtime_error In case the parent group cannot be obtained
    */
    auto Group::parent_group() const -> Group
    {
        int group_id{};

        if (auto const status = nc_inq_grp_parent(_id, &group_id); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get parent group: {}", error_message(status)));
        }

        return group_id;
    }


    /*!
        @brief      Add a child group with name @a name
        @exception  std::runtime_error In case the group cannot be added
    */
    auto Group::add_child_group(std::string const& name) const -> Group
    {
        int group_id{};

        if (auto const status = nc_def_grp(_id, name.c_str(), &group_id); status != NC_NOERR)
        {
            throw std::runtime_error(
                std::format("Cannot define child group {}: {}", name, error_message(status)));
        }

        return group_id;
    }


    /*!
        @brief      Return whether the group contains a child group with name @a name
    */
    auto Group::has_child_group(std::string const& name) const -> bool
    {
        int group_id{};

        return nc_inq_grp_ncid(_id, name.c_str(), &group_id) == NC_NOERR;
    }


    /*!
        @brief      Return the child group with name @a name
        @exception  std::runtime_error In case the child group cannot be obtained
    */
    auto Group::child_group(std::string const& name) const -> Group
    {
        int group_id{};

        if (auto const status = nc_inq_grp_ncid(_id, name.c_str(), &group_id); status != NC_NOERR)
        {
            throw std::runtime_error(
                std::format("Cannot get child group {}: {}", name, error_message(status)));
        }

        return group_id;
    }


    /*!
        @brief      Return the collection of child groups
        @exception  std::runtime_error In case the child groups cannot be obtained
    */
    auto Group::child_groups() const -> std::vector<Group>
    {
        int nr_groups{0};

        if (auto const status = nc_inq_grps(_id, &nr_groups, nullptr); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get number of groups: {}", error_message(status)));
        }

        std::vector<int> group_ids(nr_groups);

        if (auto const status = nc_inq_grps(_id, nullptr, group_ids.data()); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get group IDs: {}", error_message(status)));
        }

        // TODO C++23
        // auto groups = group_ids
        //     | std::views::transform(...)
        //     | std::ranges::to<std::vector>();

        auto groups_view =
            group_ids | std::views::transform([](auto const group_id) { return Group{group_id}; });

        std::vector<Group> groups(groups_view.begin(), groups_view.end());

        return groups;
    }

}  // namespace lue::netcdf4
