#include "lue/netcdf4/dimension.hpp"
#include "lue/netcdf4/error.hpp"
#include <array>
#include <format>


namespace lue::netcdf {

    /*!
        @brief      Construct an instance given group ID @a group_id and dimension ID @a dimension_id
    */
    Dimension::Dimension(int const group_id, int const dimension_id):

        _group_id{group_id},
        _dimension_id{dimension_id}

    {
    }


    /*!
        @brief      Return underlying dimension ID
    */
    auto Dimension::id() const -> int
    {
        return _dimension_id;
    }


    /*!
        @brief      Return underlying group ID
    */
    auto Dimension::group_id() const -> int
    {
        return _group_id;
    }


    /*!
        @brief      Return the name
        @exception  std::runtime_error In case the name cannot be obtained
    */
    auto Dimension::name() const -> std::string
    {
        std::array<char, NC_MAX_NAME + 1> buffer{};

        if (auto const status = nc_inq_dimname(_group_id, _dimension_id, buffer.data()); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get dimension name: {}", error_message(status)));
        }

        return buffer.data();
    }


    /*!
        @brief      Return the length
        @exception  std::runtime_error In case the length cannot be obtained
    */
    auto Dimension::length() const -> std::size_t
    {
        std::size_t length{0};

        if (auto const status = nc_inq_dimlen(_group_id, _dimension_id, &length); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get dimension length: {}", error_message(status)));
        }

        return length;
    }


    /*!
        @brief      Return whether @a lhs and @a rhs are equal

        Equality is defined by the underlying dimension IDs being equal.
    */
    auto operator==(Dimension const& lhs, Dimension const& rhs) -> bool
    {
        // TODO Are dimension IDs globally unique or only within the group? In the latter case, also compare
        //      group IDs.
        return lhs.id() == rhs.id();
    }

}  // namespace lue::netcdf
