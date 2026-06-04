#pragma once
#include "lue/framework/api/c/export.h"
#include "lue/framework/api/c/field.h"


#ifdef __cplusplus
extern "C" {
#endif


LUE_FCAPI_EXPORT LUE_Field* lue_abs(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_acos(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_add(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_asin(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_atan(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_atan2(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_ceil(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_close_to(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_cos(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_divide(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_equal_to(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_exp(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_floor(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_greater_than(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_greater_than_equal_to(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_less_than(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_less_than_equal_to(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_log(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_log10(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_logical_and(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_logical_exclusive_or(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_logical_inclusive_or(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_logical_not(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_modulus(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_multiply(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_negate(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_not_equal_to(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* lue_round(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_sin(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_tan(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_trunc(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* lue_where(
    LUE_Field const* field1, LUE_Field const* field2, LUE_Field const* field3 /* = nullptr */);


#ifdef __cplusplus
}
#endif
