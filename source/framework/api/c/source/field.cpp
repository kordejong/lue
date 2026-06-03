#include "field.hpp"


void destruct_field(Field* field)
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
