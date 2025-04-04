#include "lue/netcdf4/variable.hpp"
#include <ranges>


namespace lue::netcdf {

    Variable::Variable(int const group_id, int const variable_id):

        _group_id{group_id},
        _variable_id{variable_id}

    {
    }


    auto Variable::type() const -> nc_type
    {
        nc_type type{};

        if (auto const status = nc_inq_vartype(_group_id, _variable_id, &type); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get type: {}", error_message(status)));
        }

        return type;
    }


    auto Variable::nr_dimensions() const -> int
    {
        int nr_dimensions{0};

        if (auto const status = nc_inq_varndims(_group_id, _variable_id, &nr_dimensions); status != NC_NOERR)
        {
            throw std::runtime_error(
                std::format("Cannot get number of dimensions: {}", error_message(status)));
        }

        return nr_dimensions;
    }


    auto Variable::dimensions() const -> std::vector<Dimension>
    {
        int nr_dimensions{this->nr_dimensions()};
        std::vector<int> dimension_ids(nr_dimensions);

        if (auto const status = nc_inq_vardimid(_group_id, _variable_id, dimension_ids.data());
            status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get dimension IDs: {}", error_message(status)));
        }

        // TODO C++23
        // auto dimensions = dimension_ids
        //     | std::views::transform(...)
        //     | std::ranges::to<std::vector>();

        auto dimensions_view =
            dimension_ids | std::views::transform([group_id = _group_id](auto const dimension_id)
                                                  { return Dimension{group_id, dimension_id}; });

        std::vector<Dimension> dimensions(dimensions_view.begin(), dimensions_view.end());

        return dimensions;
    }


    auto Variable::name() const -> std::string
    {
        std::array<char, NC_MAX_NAME + 1> buffer{};

        if (auto const status = nc_inq_varname(_group_id, _variable_id, buffer.data()); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get variable name: {}", error_message(status)));
        }

        return buffer.data();
    }


    void Variable::unset_fill_value() const
    {
        if (auto const status = nc_def_var_fill(_group_id, _variable_id, NC_NOFILL, nullptr);
            status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot unset fill value: {}", error_message(status)));
        }
    }


    auto Variable::has_fill_value() const -> bool
    {
        int fill_mode{};

        if (auto const status = nc_inq_var_fill(_group_id, _variable_id, &fill_mode, nullptr);
            status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get fill mode: {}", error_message(status)));
        }

        return fill_mode == NC_FILL;
    }


    auto Variable::has_attribute(std::string const& name) const -> bool
    {
        int attribute_id{};

        return nc_inq_attid(_group_id, _variable_id, name.c_str(), &attribute_id) == NC_NOERR;
    }


    auto Variable::attribute(std::string name) const -> Attribute
    {
        return {_group_id, _variable_id, std::move(name)};
    }


    auto Variable::attributes() const -> std::vector<Attribute>
    {
        int nr_attributes{0};

        if (auto const status = nc_inq_varnatts(_group_id, _variable_id, &nr_attributes); status != NC_NOERR)
        {
            throw std::runtime_error(
                std::format("Cannot get number of variable attributes: {}", error_message(status)));
        }

        std::vector<Attribute> attributes;
        attributes.reserve(nr_attributes);

        for (int attribute_id = 0; attribute_id < nr_attributes; ++attribute_id)
        {
            attributes.push_back(Attribute::open(_group_id, _variable_id, attribute_id));
        }

        return attributes;
    }

}  // namespace lue::netcdf
