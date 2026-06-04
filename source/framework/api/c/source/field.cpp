#include "field.hpp"


void lue_destruct_field(LUE_Field* field)
{
    if (field != nullptr)
    {
        if (field->instance != nullptr)
        {
            delete lue_as_cxx_field_ptr(field);
        }

        delete field;
    }
}
