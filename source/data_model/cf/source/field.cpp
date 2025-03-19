#include "lue/cf/field.hpp"


namespace lue::cf {

    Field::Field(int const group_id, int const variable_id):

        netcdf4::DataVariable{group_id, variable_id}

    {
    }


    auto Field::domain() const -> std::optional<Domain> const&
    {
        return _domain;
    }

}  // namespace lue::cf
