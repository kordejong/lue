#pragma once
#include "lue/framework/api/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


struct Array
{

        /*!
            @brief      Opaque pointer to the C++ Array instance
        */
        void* instance;
};


typedef struct Array Array;


LUE_FCAPI_EXPORT void lue_destruct_array(Array* array);


#ifdef __cplusplus
}
#endif
