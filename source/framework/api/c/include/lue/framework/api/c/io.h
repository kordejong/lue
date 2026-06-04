#pragma once
#include "lue/framework/api/c/export.h"
#include "lue/framework/api/c/field.h"


#ifdef __cplusplus
extern "C" {
#endif


LUE_FCAPI_EXPORT LUE_Field* lue_from_gdal(char const* name);

LUE_FCAPI_EXPORT void lue_to_gdal(
    LUE_Field const* field, char const* name, char const* clone_name /* = nullptr */);


#ifdef __cplusplus
}
#endif
