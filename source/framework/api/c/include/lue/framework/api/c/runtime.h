#pragma once
#include "lue/framework/api/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


LUE_FCAPI_EXPORT int lue_initialize_runtime(int (*entry_point)(int, char**), int argc, char** argv);

LUE_FCAPI_EXPORT int lue_finalize_runtime();


#ifdef __cplusplus
}
#endif
