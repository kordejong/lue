#pragma once
#include "lue/cf/export.hpp"
#include "lue/netcdf4/dimension.hpp"


namespace lue::cf {

    /*!
        @brief      .
        @tparam     .
        @param      .
        @return     .
        @exception  .

        - Dimension names are not standardized
    */
    class LUE_CF_EXPORT Dimension: public netcdf::Dimension
    {

        public:

            enum class Kind {
                latitude,
                longitude,
                time,
                unknown,
            };

            Dimension(netcdf::Dimension const& dimension);

            [[nodiscard]] auto kind() const -> Kind;

            [[nodiscard]] auto is_spatiotemporal() const -> bool;

        private:

            [[nodiscard]] auto is_coordinate_dimension(std::string const& name) const -> bool;
    };

}  // namespace lue::cf
