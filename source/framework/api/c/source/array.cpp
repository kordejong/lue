#include "array.hpp"


void lue_destruct_array(Array* array)
{
    if (array != nullptr)
    {
        if (array->instance != nullptr)
        {
            delete as_cxx_array_ptr(array);
        }

        delete array;
    }
}
