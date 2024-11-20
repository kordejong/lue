#include "lue/netcdf/group.hpp"
#include "lue/netcdf/error.hpp"
#include <format>


namespace lue::netcdf {

    Group::Group(int id):

        _id{id}

    {
        // TODO Verify id is a valid ID of an open group. Don't create instances for invalid IDs.
    }

}  // namespace lue::netcdf
