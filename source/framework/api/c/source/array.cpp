#include "array.hpp"


void lue_destruct_array(LUE_Array* array)
{
    if (array != nullptr)
    {
        if (array->instance != nullptr)
        {
            delete lue_as_cxx_array_ptr(array);
        }

        delete array;
    }
}
