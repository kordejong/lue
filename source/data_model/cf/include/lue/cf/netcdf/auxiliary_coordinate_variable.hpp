#pragma once
#include "lue/cf/netcdf/generic_coordinate_variable.hpp"


namespace lue::cf::netcdf4 {

    /*!
        @brief      Additional or alternative coordinates for any axes
        @tparam     .
        @param      .
        @return     .
        @exception  .

        - Contain coordinate data
        - Values indicate the location of grid points. If bounds are not provided, an application might
            reasonably assume the grid points to be at the centers of the cells.
        - Treat data as missing where coordinates are marked as missing
    */
    class AuxiliaryCoordinateVariable: public GenericCoordinateVariable
    {

        public:

            // TODO
            // To indicate missing data, use these attributes:
            // _FillValue, missing_value, valid_min, valid_max, valid_range

        private:
    };

}  // namespace lue::cf::netcdf4
