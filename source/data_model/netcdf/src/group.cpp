#include "lue/netcdf/group.hpp"
#include "lue/netcdf/error.hpp"
#include <format>
#include <ranges>


namespace lue::netcdf {

    Group::Group(int const group_id):

        _id{group_id}

    {
    }


    auto Group::id() const -> int
    {
        return _id;
    }


    /*!
        @brief      Define a variable
        @param      name Variable name
        @param      data_type Data type
        @param      dimensions Variable dimensions
        @return     Defined variable
        @exception  std::runtime_error In case the variable cannot be defined
    */
    auto Group::define_variable(
        std::string const& name,
        nc_type const data_type,
        std::vector<Dimension> const& dimensions) const -> Variable
    {
        int variable_id{};

        // TODO C++23
        // auto dimension_ids = dimensions
        //     | std::views::transform([](auto const& dimension) { return dimension.id(); })
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
                std::format("Cannot define variable {}: {}", name, error_message(status)));
        }

        return {_id, variable_id};
    }


    template<>
    auto Group::put_attribute(std::string name, std::vector<int> const& values) -> Attribute
    {
        if (int status = nc_put_att_int(_id, NC_GLOBAL, name.c_str(), NC_INT, values.size(), values.data());
            status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot put attribute {}: {}", name, error_message(status)));
        }

        // TODO
        // An attribute is always:
        // - A member of a group (global attribute)
        // - A member of a variable (local attribute)
        // An attribute always has a name
        //
        // This member returns a global attribute
        return {_id, std::move(name)};
    }

}  // namespace lue::netcdf
