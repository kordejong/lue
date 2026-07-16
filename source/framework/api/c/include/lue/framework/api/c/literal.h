#pragma once
#include "lue/framework/api/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


struct LUE_Literal
{

        /*!
            @brief      Opaque pointer to the C++ Literal instance
        */
        void* instance;
};


typedef struct LUE_Literal LUE_Literal;


LUE_FCAPI_EXPORT void lue_destruct_literal(LUE_Literal* literal);


#ifdef __cplusplus
}
#endif
