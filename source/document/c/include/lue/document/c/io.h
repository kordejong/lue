#pragma once
#include "lue/framework/api/c.h"
#include "lue/document/c/export.h"


#ifdef __cplusplus
extern "C" {
#endif


LUE_DOCUMENT_C_EXPORT LUE_Kernel* lue_read_kernel(char const* pathname);


#ifdef __cplusplus
}
#endif
