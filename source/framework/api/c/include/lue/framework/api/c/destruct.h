#pragma once
#include "lue/framework/api/c/array.h"
#include "lue/framework/api/c/field.h"
#include "lue/framework/api/c/future.h"
#include "lue/framework/api/c/literal.h"
#include "lue/framework/api/c/scalar.h"


#ifdef __cplusplus
extern "C" {
#endif


#define lue_destruct(X)                                                                                      \
    _Generic(                                                                                                \
        (X),                                                                                                 \
        LUE_Array*: lue_destruct_array,                                                                      \
        LUE_Field*: lue_destruct_field,                                                                      \
        LUE_Future*: lue_destruct_future,                                                                    \
        LUE_Literal*: lue_destruct_literal,                                                                  \
        LUE_Scalar*: lue_destruct_scalar)(X)


#ifdef __cplusplus
}
#endif
