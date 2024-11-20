#include "lue/netcdf/variable.hpp"
// #include "lue/netcdf/error.hpp"
// #include <format>


namespace lue::netcdf {

    Variable::Variable(int const group_id, int const variable_id):

        _group_id{group_id},
        _variable_id{variable_id}

    {
    }

}  // namespace lue::netcdf
