#include "lue/netcdf4/dimension.hpp"
#include "lue/netcdf4/error.hpp"
#include <array>
#include <format>


namespace lue::netcdf {

    Dimension::Dimension(int const group_id, int const dimension_id):

        _group_id{group_id},
        _dimension_id{dimension_id}

    {
    }


    auto Dimension::id() const -> int
    {
        return _dimension_id;
    }


    auto Dimension::group_id() const -> int
    {
        return _group_id;
    }


    auto Dimension::name() const -> std::string
    {
        std::array<char, NC_MAX_NAME + 1> buffer{};

        if (auto const status = nc_inq_dimname(_group_id, _dimension_id, buffer.data()); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get dimension name: {}", error_message(status)));
        }

        return buffer.data();
    }


    auto Dimension::length() const -> std::size_t
    {
        std::size_t length{0};

        if (auto const status = nc_inq_dimlen(_group_id, _dimension_id, &length); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get dimension length: {}", error_message(status)));
        }

        return length;
    }


    auto operator==(Dimension const& lhs, Dimension const& rhs) -> bool
    {
        // TODO Are dimension IDs globally unique or only within the group? In the latter case, also compare
        //      group IDs.
        return lhs.id() == rhs.id();
    }

}  // namespace lue::netcdf
