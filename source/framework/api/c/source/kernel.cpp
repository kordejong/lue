#include "kernel.hpp"


void lue_destruct_boolean_kernel(LUE_BooleanKernel* kernel)
{
    if (kernel != nullptr)
    {
        if (kernel->instance != nullptr)
        {
            delete lue_as_cxx_boolean_kernel_ptr(kernel);
        }

        delete kernel;
    }
}
