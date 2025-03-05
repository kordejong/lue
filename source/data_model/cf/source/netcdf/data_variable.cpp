#include "lue/cf/netcdf/data_variable.hpp"


namespace lue::cf::netcdf {

    DataVariable::DataVariable(int const group_id, int const variable_id):

        lue::netcdf4::Variable{group_id, variable_id}

    {
    }

}  // namespace lue::cf::netcdf
