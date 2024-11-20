#include "lue/netcdf/attribute.hpp"
#include "lue/netcdf/error.hpp"
#include <format>


namespace lue::netcdf {

    Attribute::Attribute(int id):

        _id{id}

    {
        // TODO Verify id is a valid ID of an open attribute. Don't create instances for invalid IDs.
    }

}  // namespace lue::netcdf
