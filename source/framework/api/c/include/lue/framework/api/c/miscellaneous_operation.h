#pragma once
#include "lue/framework/api/c/export.h"
#include "lue/framework/api/c/field.h"
#include "lue/framework/api/c/literal.h"
#include "math.h"
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef size_t Rank;
typedef ptrdiff_t Count;

LUE_FCAPI_EXPORT Field* create_array(Rank rank, Count const* array_shape, Field const* fill_value);

LUE_FCAPI_EXPORT Field* create_scalar(Literal* value);

LUE_FCAPI_EXPORT Field* create_literal_float32(float value);

LUE_FCAPI_EXPORT Field* create_literal_float64(double value);

#define create_literal(X) _Generic((X), float: create_literal_float32, double: create_literal_float64)(X)

#ifdef __cplusplus
}
#endif
