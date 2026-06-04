#pragma once
#include "lue/framework/api/c/export.h"
#include "lue/framework/api/c/field.h"
#include "lue/framework/api/c/literal.h"
#include "lue/framework/api/c/scalar.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    LUE_DataModel_Array,
    LUE_DataModel_Scalar,
    LUE_DataModel_Literal,
} LUE_DataModel;


typedef enum {
    LUE_ElementType_UInt8,
    LUE_ElementType_UInt16,
    LUE_ElementType_UInt32,
    LUE_ElementType_UInt64,
    LUE_ElementType_Int8,
    LUE_ElementType_Int16,
    LUE_ElementType_Int32,
    LUE_ElementType_Int64,
    LUE_ElementType_Float32,
    LUE_ElementType_Float64,
} LUE_ElementType;


LUE_FCAPI_EXPORT LUE_DataModel lue_data_model(Field const* field);

LUE_FCAPI_EXPORT LUE_ElementType lue_element_type_field(Field const* field);

LUE_FCAPI_EXPORT LUE_ElementType lue_element_type_literal(Literal const* literal);

LUE_FCAPI_EXPORT LUE_ElementType lue_element_type_scalar(Scalar const* scalar);

#define lue_element_type(X)                                                                                  \
    _Generic(                                                                                                \
        (X),                                                                                                 \
        Field*: lue_element_type_field,                                                                      \
        Literal*: lue_element_type_literal,                                                                  \
        Scalar*: lue_element_type_scalar)(X)


#ifdef __cplusplus
}
#endif
