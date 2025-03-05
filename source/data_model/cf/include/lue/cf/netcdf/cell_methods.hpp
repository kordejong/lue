#pragma once
#include "lue/netcdf4/attribute.hpp"
#include "lue/netcdf4/dimension.hpp"
#include <vector>


namespace lue::cf::netcdf {

    class CellMethods: public lue::netcdf4::Attribute
    {

        public:

        private:

            std::vector<lue::netcdf4::Dimension> _dimensions;
    };

}  // namespace lue::cf::netcdf
