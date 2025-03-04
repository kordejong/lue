#include "lue/netcdf4/error.hpp"
#include <netcdf.h>


namespace lue::netcdf {

    /*!
        @brief      Return error message associated with @a status
    */
    auto error_message(int const status) -> std::string
    {
        return nc_strerror(status);
    }

}  // namespace lue::netcdf
