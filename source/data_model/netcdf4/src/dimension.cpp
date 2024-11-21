#include "lue/netcdf4/dimension.hpp"


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
