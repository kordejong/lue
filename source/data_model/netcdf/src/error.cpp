#include "lue/netcdf/error.hpp"
#include <netcdf.h>


namespace lue::netcdf {

    auto error_message(int const status) -> std::string
    {
        return nc_strerror(status);
    }

}  // namespace lue::netcdf
