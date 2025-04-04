#pragma once
#include "lue/cf/export.hpp"
#include "lue/netcdf4/variable.hpp"


namespace lue::cf {

    /*!
        @brief      .
        @tparam     .
        @param      .
        @return     .
        @exception  .

        - Variable names are not standardized
    */
    class LUE_CF_EXPORT Variable: public netcdf::Variable
    {

        public:

            enum class Kind {
                // auxiliary_coordinate,
                // scalar_coordinate,
                // multidimensional_coordinate,
                coordinate,
                regular,
            };

            // static auto is_coordinate(netcdf::Variable const& variable) -> bool;

            // static auto is_scalar(netcdf::Variable const& variable) -> bool;

            Variable(netcdf::Variable const& variable);

            [[nodiscard]] auto standard_name() const -> std::string;

            [[nodiscard]] auto long_name() const -> std::string;

            [[nodiscard]] auto units() const -> std::string;

            [[nodiscard]] auto kind() const -> Kind;
    };

}  // namespace lue::cf
