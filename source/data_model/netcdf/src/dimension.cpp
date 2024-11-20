#include "lue/netcdf/dimension.hpp"
#include "lue/netcdf/error.hpp"
#include <format>


namespace lue::netcdf {

    Dimension::Dimension(int id):

        _id{id}

    {
        // TODO Verify id is a valid ID of an open dimension. Don't create instances for invalid IDs.
    }

}  // namespace lue::netcdf
