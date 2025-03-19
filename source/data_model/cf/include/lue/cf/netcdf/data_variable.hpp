#pragma once
#include "lue/netcdf4/variable.hpp"


namespace lue::cf::netcdf4 {

    /*!
        @brief      Scientific data discretized within a domain
        @tparam     .
        @param      .
        @return     .
        @exception  .

        - Variable names are not standardized
        - Elements must have one of these types: string, char, integral, floating point
        - ...

        These variables, if present, override the attribute in the dataset group:

        - `institution`
        - `source`
        - `comment`
        - `references`
    */
    class DataVariable: public lue::netcdf4::Variable
    {

            // TODO
            // - Missing data is supported. Use these attributes:
            // - _FillValue, missing_value, valid_min, valid_max, valid_range

        public:

            DataVariable(int group_id, int variable_id);

            [[nodiscard]] auto standard_name() const -> std::string;

            [[nodiscard]] auto long_name() const -> std::string;

            [[nodiscard]] auto units() const -> std::string;

        private:

            // TODO
            // To indicate missing data, these attributes:
            // _FillValue, missing_value, valid_min, valid_max, valid_range


            // [[nodiscard]] auto kind() const -> Kind;

            // [[nodiscard]] auto axis() const -> std::string;

            // [[nodiscard]] auto positive() const -> std::string;
    };

}  // namespace lue::cf::netcdf4
