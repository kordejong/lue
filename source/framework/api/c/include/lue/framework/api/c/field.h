#pragma once
#include "lue/framework/api/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


struct LUE_Field
{

        /*!
            @brief      Opaque pointer to the C++ Field instance
        */
        void* instance;
};


typedef struct LUE_Field LUE_Field;


LUE_FCAPI_EXPORT void lue_destruct_field(LUE_Field* field);


#ifdef __cplusplus
}
#endif
