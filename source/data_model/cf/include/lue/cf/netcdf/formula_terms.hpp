#pragma once
#include "lue/netcdf4/attribute.hpp"
#include "lue/netcdf4/variable.hpp"


namespace lue::cf::netcdf {

    class FormulaTerms: public lue::netcdf::Attribute
    {

        public:

        private:

            // 1+ variables
            std::vector<lue::netcdf::Variable> _variables;
    };

}  // namespace lue::cf::netcdf
