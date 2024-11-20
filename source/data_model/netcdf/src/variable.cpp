#include "lue/netcdf/variable.hpp"
#include "lue/netcdf/error.hpp"
#include <format>


namespace lue::netcdf {

    Variable::Variable(int id):

        _id{id}

    {
        // TODO Verify id is a valid ID of an open variable. Don't create instances for invalid IDs.
    }

}  // namespace lue::netcdf
