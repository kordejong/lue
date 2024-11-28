#include "lue/netcdf4/group.hpp"
#include "lue/netcdf4/error.hpp"
#include <ranges>


namespace lue::netcdf {

    Group::Group(int const group_id):

        _id{group_id}

    {
    }


    Group::Group(Group&& other) noexcept:
        _id{other._id}
    {
        other._id = -1;
    }


    auto Group::id() const -> int
    {
        return _id;
    }


    auto Group::add_dimension(std::string const& name, size_t const length) const -> Dimension
    {
        int dimension_id{};

        if (int status = nc_def_dim(_id, name.c_str(), length, &dimension_id); status != NC_NOERR)
        {
            throw std::runtime_error(
                fmt::format("Cannot define dimension {}: {}", name, error_message(status)));
        }

        return {_id, dimension_id};
    }


    auto Group::has_dimension(std::string const& name) const -> bool
    {
        int dimension_id{};

        return nc_inq_dimid(_id, name.c_str(), &dimension_id) == NC_NOERR;
    }


    auto Group::dimension(std::string const& name) const -> Dimension
    {
        int dimension_id{};

        if (int status = nc_inq_dimid(_id, name.c_str(), &dimension_id); status != NC_NOERR)
        {
            throw std::runtime_error(fmt::format("Cannot get dimension {}: {}", name, error_message(status)));
        }

        return {_id, dimension_id};
    }


    auto Group::nr_dimensions() const -> int
    {
        int nr_dimensions{0};

        if (int status = nc_inq_ndims(_id, &nr_dimensions); status != NC_NOERR)
        {
            throw std::runtime_error(
                fmt::format("Cannot get number of dimensions: {}", error_message(status)));
        }

        return nr_dimensions;
    }


    /*!
        @brief      Define a variable
        @param      name Variable name
        @param      data_type Data type
        @param      dimensions Variable dimensions
        @return     Defined variable
        @exception  std::runtime_error In case the variable cannot be defined

        A default fill value will be set (as per netCDF convention), which is dependent on the data type.
    */
    auto Group::add_variable(
        std::string const& name,
        nc_type const data_type,
        std::vector<Dimension> const& dimensions) const -> Variable
    {
        int variable_id{};

        // TODO C++23
        // auto dimension_ids = dimensions
        //     | std::views::transform(...)
        //     | std::ranges::to<std::vector>();

        auto dimension_ids_view =
            dimensions | std::views::transform([](auto const& dimension) { return dimension.id(); });

        std::vector<int> dimension_ids(dimension_ids_view.begin(), dimension_ids_view.end());

        if (int status = nc_def_var(
                _id,
                name.c_str(),
                data_type,
                static_cast<int>(dimension_ids.size()),
                dimension_ids.data(),
                &variable_id);
            status != NC_NOERR)
        {
            throw std::runtime_error(
                fmt::format("Cannot define variable {}: {}", name, error_message(status)));
        }

        Variable variable{_id, variable_id};

        // If this fails, update the doc
        assert(variable.has_fill_value());

        return variable;
    }


    auto Group::has_variable(std::string const& name) const -> bool
    {
        int variable_id{};

        return nc_inq_varid(_id, name.c_str(), &variable_id) == NC_NOERR;
    }


    auto Group::variable(std::string const& name) const -> Variable
    {
        int variable_id{};

        if (int status = nc_inq_varid(_id, name.c_str(), &variable_id); status != NC_NOERR)
        {
            throw std::runtime_error(fmt::format("Cannot get variable {}: {}", name, error_message(status)));
        }

        return {_id, variable_id};
    }


    auto Group::variables() const -> std::vector<Variable>
    {
        int nr_variables{0};

        if (int status = nc_inq_varids(_id, &nr_variables, nullptr); status != NC_NOERR)
        {
            throw std::runtime_error(
                fmt::format("Cannot get number of variables: {}", error_message(status)));
        }

        std::vector<int> variable_ids(nr_variables);

        if (int status = nc_inq_varids(_id, nullptr, variable_ids.data()); status != NC_NOERR)
        {
            throw std::runtime_error(fmt::format("Cannot get variable IDs: {}", error_message(status)));
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
        @brief      Define a sub-group
        @param      .
        @return     .
        @exception  .
    */
    auto Group::add_sub_group(std::string const& name) const -> Group
    {
        int group_id{};

        if (int status = nc_def_grp(_id, name.c_str(), &group_id); status != NC_NOERR)
        {
            throw std::runtime_error(fmt::format("Cannot define group {}: {}", name, error_message(status)));
        }

        return group_id;
    }


    auto Group::has_sub_group(std::string const& name) const -> bool
    {
        int group_id{};

        return nc_inq_grp_ncid(_id, name.c_str(), &group_id) == NC_NOERR;
    }


    auto Group::sub_group(std::string const& name) const -> Group
    {
        int group_id{};

        if (int status = nc_inq_grp_ncid(_id, name.c_str(), &group_id); status != NC_NOERR)
        {
            throw std::runtime_error(fmt::format("Cannot get sub-group {}: {}", name, error_message(status)));
        }

        return group_id;
    }

}  // namespace lue::netcdf
