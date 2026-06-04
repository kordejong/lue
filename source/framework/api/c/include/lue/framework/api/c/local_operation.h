#pragma once
#include "lue/framework/api/c/export.h"
#include "lue/framework/api/c/field.h"


#ifdef __cplusplus
extern "C" {
#endif


LUE_FCAPI_EXPORT LUE_Field* abs_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* acos_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* add(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* asin_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* atan_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* atan2_(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* ceil_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* close_to(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* cos_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* divide(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* equal_to(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* exp_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* floor_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* greater_than(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* greater_than_equal_to(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* less_than(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* less_than_equal_to(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* log_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* log10_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* logical_and(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* logical_exclusive_or(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* logical_inclusive_or(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* logical_not(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* modulus(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* multiply(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* negate(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* not_equal_to(LUE_Field const* field1, LUE_Field const* field2);

LUE_FCAPI_EXPORT LUE_Field* round_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* sin_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* tan_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* trunc_(LUE_Field const* field);

LUE_FCAPI_EXPORT LUE_Field* where(
    LUE_Field const* field1, LUE_Field const* field2, LUE_Field const* field3 /* = nullptr */);


#ifdef __cplusplus
}
#endif
