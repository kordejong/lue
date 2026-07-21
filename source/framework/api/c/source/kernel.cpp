#include "kernel.hpp"


void lue_destruct_kernel(LUE_Kernel* kernel)
{
    if (kernel != nullptr)
    {
        if (kernel->instance != nullptr)
        {
            delete lue_as_cxx_kernel_ptr(kernel);
        }

        delete kernel;
    }
}
