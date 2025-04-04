#pragma once
#include "lue/netcdf4/variable.hpp"


namespace lue::cf::netcdf {

    /*!
        @brief      .
        @tparam     .
        @param      .
        @return     .
        @exception  .

        - Variable names are not standardized
        - Elements must have one of these types: string, char, integral, floating point
        - ...
    */
    class DataVariable: public lue::netcdf::Variable
    {

        public:

            DataVariable(int group_id, int variable_id);

        private:

            [[nodiscard]] auto standard_name() const -> std::string;

            [[nodiscard]] auto long_name() const -> std::string;

            [[nodiscard]] auto units() const -> std::string;

            // TODO
            // To indicate missing data, these attributes:
            // _FillValue, missing_value, valid_min, valid_max, valid_range


            // [[nodiscard]] auto kind() const -> Kind;

            // [[nodiscard]] auto axis() const -> std::string;

            // [[nodiscard]] auto positive() const -> std::string;
    };

}  // namespace lue::cf::netcdf
