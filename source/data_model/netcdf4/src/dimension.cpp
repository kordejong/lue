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


    auto Dimension::name() const -> std::string
    {
        std::array<char, NC_MAX_NAME + 1> buffer{};

        if (int status = nc_inq_dimname(_group_id, _dimension_id, buffer.data()); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get dimension name: {}", error_message(status)));
        }

        return buffer.data();
    }


    auto Dimension::length() const -> std::size_t
    {
        std::size_t length{0};

        if (int status = nc_inq_dimlen(_group_id, _dimension_id, &length); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get dimension length: {}", error_message(status)));
        }

        return length;
    }

}  // namespace lue::netcdf
