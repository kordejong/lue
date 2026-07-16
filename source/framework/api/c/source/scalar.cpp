#include "scalar.hpp"


void lue_destruct_scalar(LUE_Scalar* scalar)
{
    if (scalar != nullptr)
    {
        if (scalar->instance != nullptr)
        {
            delete lue_as_cxx_scalar_ptr(scalar);
        }

        delete scalar;
    }
}
