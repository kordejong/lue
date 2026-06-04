#include "field.hpp"


void lue_destruct_field(Field* field)
{
    if (field != nullptr)
    {
        if (field->instance != nullptr)
        {
            delete as_cxx_field_ptr(field);
        }

        delete field;
    }
}
