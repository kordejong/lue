#pragma once
#include "lue/framework/api/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


struct Scalar
{

        /*!
            @brief      Opaque pointer to the C++ Scalar instance
        */
        void* instance;
};


typedef struct Scalar Scalar;


LUE_FCAPI_EXPORT void destruct_scalar(Scalar* scalar);


#ifdef __cplusplus
}
#endif
