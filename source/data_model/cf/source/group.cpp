#include "lue/cf/group.hpp"


namespace lue::cf {

    Group::Group(netcdf::Group const& group):

        netcdf::Group{group}

    {
    }

}  // namespace lue::cf
