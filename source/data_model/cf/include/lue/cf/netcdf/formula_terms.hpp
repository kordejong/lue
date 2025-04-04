#pragma once
#include "lue/netcdf4/attribute.hpp"
#include "lue/netcdf4/variable.hpp"


namespace lue::cf::netcdf4 {

    /*!
        @brief      Vertical coordinate system
        @tparam     .
        @param      .
        @return     .
        @exception  .
    */
    class FormulaTerms: public lue::netcdf4::Attribute
    {

        public:

        private:

            // 1+ variables
            std::vector<lue::netcdf4::Variable> _variables;
    };

}  // namespace lue::cf::netcdf4
