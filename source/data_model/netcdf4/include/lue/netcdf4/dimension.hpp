#pragma once
#include "lue/netcdf4/export.hpp"
#include <netcdf.h>
#include <string>


namespace lue::netcdf4 {

    /*!
        @brief      A class for representing dimensions

        A dimension defines a single axes for quantities contained by it.
    */
    class LUE_NETCDF4_EXPORT Dimension
    {

        public:

            Dimension(int group_id, int dimension_id);

            [[nodiscard]] auto id() const -> int;

            [[nodiscard]] auto group_id() const -> int;

            [[nodiscard]] auto name() const -> std::string;

            [[nodiscard]] auto length() const -> std::size_t;

        private:

            //! ID of the group
            int _group_id;

            //! ID of the variable
            int _dimension_id;
    };


    auto operator==(Dimension const& lhs, Dimension const& rhs) -> bool;

}  // namespace lue::netcdf4
