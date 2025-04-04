#pragma once
#include "lue/cf/export.hpp"
#include "lue/netcdf4/group.hpp"


namespace lue::cf {

    class LUE_CF_EXPORT Group: public netcdf::Group
    {

        public:

            Group(netcdf::Group const& group);
    };

}  // namespace lue::cf
