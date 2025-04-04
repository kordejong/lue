#pragma once
#include "lue/cf/netcdf/generic_coordinate_variable.hpp"


namespace lue::cf::netcdf {

    /*!
        @brief      .
        @tparam     .
        @param      .
        @return     .
        @exception  .

        - Contain coordinate data
        - Treat data as missing where coordinates are marked as missing
    */
    class AuxiliaryCoordinateVariable: public GenericCoordinateVariable
    {

        public:

            // TODO
            // To indicate missing data, these attributes:
            // _FillValue, missing_value, valid_min, valid_max, valid_range

        private:
    };

}  // namespace lue::cf::netcdf
