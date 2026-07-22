#pragma once
#include "lue/framework/api/c/export.h"
#include "lue/framework/api/c/field.h"
#include "lue/framework/api/c/kernel.h"
#include "lue/framework/api/c/literal.h"


#ifdef __cplusplus
extern "C" {
#endif


LUE_FCAPI_EXPORT LUE_Field* lue_aspect(LUE_Field const* elevation);

LUE_FCAPI_EXPORT LUE_Field* lue_convolve(LUE_Field const* field, LUE_Kernel const* kernel);

LUE_FCAPI_EXPORT LUE_Field* lue_focal_diversity(LUE_Field const* field, LUE_Kernel const* kernel);

LUE_FCAPI_EXPORT LUE_Field* lue_focal_high_pass(LUE_Field const* field, LUE_Kernel const* kernel);

LUE_FCAPI_EXPORT LUE_Field* lue_focal_majority(LUE_Field const* field, LUE_Kernel const* kernel);

LUE_FCAPI_EXPORT LUE_Field* lue_focal_maximum(LUE_Field const* field, LUE_Kernel const* kernel);

LUE_FCAPI_EXPORT LUE_Field* lue_focal_mean(LUE_Field const* field, LUE_Kernel const* kernel);

LUE_FCAPI_EXPORT LUE_Field* lue_focal_minimum(LUE_Field const* field, LUE_Kernel const* kernel);

LUE_FCAPI_EXPORT LUE_Field* lue_focal_sum(LUE_Field const* field, LUE_Kernel const* kernel);

LUE_FCAPI_EXPORT LUE_Field* lue_slope(LUE_Field const* elevation, LUE_Literal const* cell_size);


#ifdef __cplusplus
}
#endif
