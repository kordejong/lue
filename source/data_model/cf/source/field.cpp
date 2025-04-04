#include "lue/cf/field.hpp"


namespace lue::cf {

    Field::Field(int const group_id, int const variable_id):

        netcdf::DataVariable{group_id, variable_id}

    {
    }

}  // namespace lue::cf
