#pragma once
#include "lue/framework/api/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


struct LUE_Future
{

        /*!
            @brief      Opaque pointer to the C++ lue::api::Future instance
        */
        void* instance;
};


typedef struct LUE_Future LUE_Future;


LUE_FCAPI_EXPORT void lue_destruct_future(LUE_Future* future);


#ifdef __cplusplus
}
#endif
