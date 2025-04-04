#pragma once
#include "lue/cf/netcdf/generic_coordinate_variable.hpp"


namespace lue::cf::netcdf4 {

    /*!
        @brief      Unique coordinates for a single axis
        @tparam     .
        @param      .
        @return     .
        @exception  .

        - Contain coordinate data
        - Values indicate the location of grid points. If bounds are not provided, an application might
            reasonably assume the grid points to be at the centers of the cells.
        - Missing data is not allowed
        - Attributes:
            - units
            - positive
            - standard_name
            - formula_terms
            - axis (X, Y, Z, T)
        - Examples:
            - latitude
            - longitude
            - time
        - Coordinate bounds cannot exist without coordinates. They can therefore be grouped into a single
            element.
    */
    class CoordinateVariable: public GenericCoordinateVariable
    {

        public:

        private:
    };

}  // namespace lue::cf::netcdf4
