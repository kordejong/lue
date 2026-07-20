#pragma once
#include "lue/framework/api/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


struct LUE_BooleanKernel
{

        /*!
            @brief      Opaque pointer to the C++ BooleanKernel instance
        */
        void* instance;
};


typedef struct LUE_BooleanKernel LUE_BooleanKernel;


LUE_FCAPI_EXPORT void lue_destruct_boolean_kernel(LUE_BooleanKernel* kernel);


#ifdef __cplusplus
}
#endif
