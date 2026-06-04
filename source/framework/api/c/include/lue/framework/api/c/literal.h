#pragma once
#include "lue/framework/api/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


struct Literal
{

        /*!
            @brief      Opaque pointer to the C++ Literal instance
        */
        void* instance;
};


typedef struct Literal Literal;


LUE_FCAPI_EXPORT void lue_destruct_literal(Literal* literal);


#ifdef __cplusplus
}
#endif
