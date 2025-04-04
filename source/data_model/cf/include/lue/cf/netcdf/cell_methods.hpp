#pragma once
#include "lue/netcdf4/attribute.hpp"
#include "lue/netcdf4/dimension.hpp"
#include <vector>


namespace lue::cf::netcdf {

    class CellMethods: public lue::netcdf::Attribute
    {

        public:

        private:

            std::vector<lue::netcdf::Dimension> _dimensions;
    };

}  // namespace lue::cf::netcdf
