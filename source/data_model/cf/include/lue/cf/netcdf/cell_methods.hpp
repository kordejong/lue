#pragma once
#include "lue/netcdf4/attribute.hpp"
#include "lue/netcdf4/dimension.hpp"
#include <vector>


namespace lue::cf::netcdf4 {

    /*!
        @brief      Description of variation within cells
        @tparam     .
        @param      .
        @return     .
        @exception  .
    */
    class CellMethods: public lue::netcdf4::Attribute
    {

        public:

        private:

            // zero or more
            std::vector<lue::netcdf4::Dimension> _dimensions;
    };

}  // namespace lue::cf::netcdf4
