#include "lue/netcdf4/group.hpp"
#include "lue/netcdf4/error.hpp"
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
                fmt::format("Cannot define variable {}: {}", name, error_message(status)));
        }

        return {_id, variable_id};
    }


}  // namespace lue::netcdf
