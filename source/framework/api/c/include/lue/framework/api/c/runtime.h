#pragma once
#include "lue/framework/api/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


struct Runtime
{

        /*!
            @brief      Opaque pointer to the C++ Runtime instance
        */
        void* instance;
};


typedef struct Runtime Runtime;


LUE_FCAPI_EXPORT Runtime* construct_runtime(
    int argc, char* argv[], int nr_items, char* configuration_items[]);

LUE_FCAPI_EXPORT void destruct_runtime(Runtime* runtime);

LUE_FCAPI_EXPORT bool start_runtime(Runtime* runtime);

LUE_FCAPI_EXPORT int stop_runtime(Runtime* runtime);

LUE_FCAPI_EXPORT bool on_root_locality();

LUE_FCAPI_EXPORT void sieve_hpx_arguments(int argc, char* argv[], int* argc_new, char*** argv_new);


#ifdef __cplusplus
}
#endif
