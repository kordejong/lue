#include "lue/cf/field.hpp"


namespace lue::cf {

    Field::Field(int const group_id, int const variable_id):

        netcdf4::DataVariable{group_id, variable_id}

    {
    }


    auto Field::has_domain() const -> bool
    {
        return _domain.operator bool();
    }


    auto Field::domain() const -> Domain const&
    {
        assert(_domain);

        return *_domain;
    }


    auto Field::field_ancillaries() const -> FieldAncillaries const&
    {
        return _field_ancillaries;
    }


    auto Field::cell_methods() const -> CellMethods const&
    {
        return _cell_methods;
    }


    auto Field::properties() const -> Properties const&
    {
        return _properties;
    }

}  // namespace lue::cf
