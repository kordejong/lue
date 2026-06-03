#include "scalar.hpp"


void destruct_scalar(Scalar* scalar)
{
    if (scalar != nullptr)
    {
        if (scalar->instance != nullptr)
        {
            delete as_cxx_scalar_ptr(scalar);
        }

        delete scalar;
    }
}
