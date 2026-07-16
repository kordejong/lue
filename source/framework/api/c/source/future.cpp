#include "lue/framework/api/c/future.h"
#include "future.hpp"
#include "lue/framework/api/cxx/future.hpp"


void lue_destruct_future(LUE_Future* future)
{
    if (future != nullptr)
    {
        if (future->instance != nullptr)
        {
            delete lue_as_cxx_future_ptr(future);
        }

        delete future;
    }
}
