#pragma once
#include "lue/cf/netcdf/boundary_variable.hpp"
#include <optional>


namespace lue::cf::netcdf {

    class GenericCoordinateVariable: public lue::netcdf4::Variable
    {

        public:

        private:

            std::optional<BoundaryVariable> _boundary;
    };

}  // namespace lue::cf::netcdf
