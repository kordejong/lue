#pragma once
#include "lue/framework/api/c/export.h"
#include "lue/framework/api/c/field.h"
#include "lue/framework/api/c/kernel.h"


#ifdef __cplusplus
extern "C" {
#endif


LUE_FCAPI_EXPORT LUE_Field* lue_focal_mean(LUE_Field const* field, LUE_BooleanKernel const* kernel);


#ifdef __cplusplus
}
#endif
