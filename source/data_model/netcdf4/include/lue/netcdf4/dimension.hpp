#pragma once
#include "lue/netcdf4/export.hpp"
#include <netcdf.h>
#include <string>


namespace lue::netcdf {

    class LUE_NETCDF4_EXPORT Dimension
    {

        public:

            Dimension(int group_id, int dimension_id);

            Dimension(Dimension const& other) = default;

            Dimension(Dimension&& other) noexcept = default;

            ~Dimension() = default;

            auto operator=(Dimension const& other) -> Dimension& = default;

            auto operator=(Dimension&& other) noexcept -> Dimension& = default;

            [[nodiscard]] auto id() const -> int;

            [[nodiscard]] auto name() const -> std::string;

            [[nodiscard]] auto length() const -> std::size_t;

        private:

            //! ID of the group
            int _group_id;

            //! ID of the variable
            int _dimension_id;
    };

}  // namespace lue::netcdf
