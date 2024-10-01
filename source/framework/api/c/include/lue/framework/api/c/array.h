#pragma once
#include "lue/framework/api/c/export.h"
#include "lue/framework/api/c/field.h"
#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t Rank;

typedef ptrdiff_t Extent;

LUE_FCAPI_EXPORT Field* create_array_int32(
    Rank rank, Extent* array_extents, Field const* fill_value, Extent* partition_extents = nullptr);

#ifdef __cplusplus
}
#endif
