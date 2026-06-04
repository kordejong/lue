#pragma once
#include "lue/framework/api/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


struct LUE_Array
{

        /*!
            @brief      Opaque pointer to the C++ Array instance
        */
        void* instance;
};


typedef struct LUE_Array LUE_Array;


LUE_FCAPI_EXPORT void lue_destruct_array(LUE_Array* array);


#ifdef __cplusplus
}
#endif
