#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#define lue_destruct(X)                                                                                      \
    _Generic(                                                                                                \
        (X),                                                                                                 \
        Array*: lue_destruct_array,                                                                          \
        Field*: lue_destruct_field,                                                                          \
        Literal*: lue_destruct_literal,                                                                      \
        Scalar*: lue_destruct_scalar)(X)


#ifdef __cplusplus
}
#endif
