#pragma once
#include "lue/framework/api/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


struct LUE_Kernel
{

        /*!
            @brief      Opaque pointer to the C++ Kernel instance
        */
        void* instance;
};


typedef struct LUE_Kernel LUE_Kernel;


LUE_FCAPI_EXPORT void lue_destruct_kernel(LUE_Kernel* kernel);


#ifdef __cplusplus
}
#endif
