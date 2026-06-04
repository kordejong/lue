#pragma once
#include "lue/framework/api/c/export.h"
#include "lue/framework/api/c/field.h"
#include "lue/framework/api/c/literal.h"
#include "lue/framework/api/c/scalar.h"
#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef size_t Rank;
typedef ptrdiff_t Count;

LUE_FCAPI_EXPORT Field* create_array(Rank rank, Count const* array_shape, Scalar const* fill_value);

LUE_FCAPI_EXPORT Literal* create_literal_uint8(uint8_t value);
LUE_FCAPI_EXPORT Literal* create_literal_uint16(uint16_t value);
LUE_FCAPI_EXPORT Literal* create_literal_uint32(uint32_t value);
LUE_FCAPI_EXPORT Literal* create_literal_uint64(uint64_t value);
LUE_FCAPI_EXPORT Literal* create_literal_int8(int8_t value);
LUE_FCAPI_EXPORT Literal* create_literal_int16(int16_t value);
LUE_FCAPI_EXPORT Literal* create_literal_int32(int32_t value);
LUE_FCAPI_EXPORT Literal* create_literal_int64(int64_t value);
LUE_FCAPI_EXPORT Literal* create_literal_float32(float value);
LUE_FCAPI_EXPORT Literal* create_literal_float64(double value);

#define create_literal(X)                                                                                    \
    _Generic(                                                                                                \
        (X),                                                                                                 \
        uint8_t: create_literal_uint8,                                                                       \
        uint16_t: create_literal_uint16,                                                                     \
        uint32_t: create_literal_uint32,                                                                     \
        uint64_t: create_literal_uint64,                                                                     \
        int8_t: create_literal_int8,                                                                         \
        int16_t: create_literal_int16,                                                                       \
        int32_t: create_literal_int32,                                                                       \
        int64_t: create_literal_int64,                                                                       \
        float: create_literal_float32,                                                                       \
        double: create_literal_float64)(X)

LUE_FCAPI_EXPORT Scalar* create_scalar(Literal* value);

#ifdef __cplusplus
}
#endif
