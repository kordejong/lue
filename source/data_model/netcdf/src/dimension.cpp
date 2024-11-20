#include "lue/netcdf/dimension.hpp"
// #include "lue/netcdf/error.hpp"
// #include <format>


namespace lue::netcdf {

    Dimension::Dimension(int const dimension_id):

        _id{dimension_id}

    {
    }


    auto Dimension::id() const -> int
    {
        return _id;
    }

}  // namespace lue::netcdf
