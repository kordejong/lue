#pragma once
#include "lue/framework/api/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


struct LUE_Scalar
{

        /*!
            @brief      Opaque pointer to the C++ Scalar instance
        */
        void* instance;
};


typedef struct LUE_Scalar LUE_Scalar;


LUE_FCAPI_EXPORT void lue_destruct_scalar(LUE_Scalar* scalar);


#ifdef __cplusplus
}
#endif
