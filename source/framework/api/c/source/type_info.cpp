#include "lue/framework/api/c/type_info.h"
#include "array.hpp"
#include "field.hpp"
#include "literal.hpp"
#include "scalar.hpp"
#include "lue/framework/api/cxx/type_info.hpp"


auto lue_data_model(LUE_Field const* field) -> LUE_DataModel
{
    auto scoped_enum = lue::api::data_model(lue_as_cxx_field(field));
    LUE_DataModel result{};

    switch (scoped_enum)
    {
        case lue::DataModel::Array:
        {
            result = LUE_DataModel_Array;
            break;
        }
        case lue::DataModel::Literal:
        {
            result = LUE_DataModel_Literal;
            break;
        }
        case lue::DataModel::Scalar:
        {
            result = LUE_DataModel_Scalar;
            break;
        }
    }

    return result;
}


static auto element_type(lue::ElementType const scoped_enum)
{
    LUE_ElementType result{};

    switch (scoped_enum)
    {
        case lue::ElementType::UInt8:
        {
            result = LUE_ElementType_UInt8;
            break;
        }
        case lue::ElementType::UInt16:
        {
            result = LUE_ElementType_UInt16;
            break;
        }
        case lue::ElementType::UInt32:
        {
            result = LUE_ElementType_UInt32;
            break;
        }
        case lue::ElementType::UInt64:
        {
            result = LUE_ElementType_UInt64;
            break;
        }
        case lue::ElementType::Int8:
        {
            result = LUE_ElementType_Int8;
            break;
        }
        case lue::ElementType::Int16:
        {
            result = LUE_ElementType_Int16;
            break;
        }
        case lue::ElementType::Int32:
        {
            result = LUE_ElementType_Int32;
            break;
        }
        case lue::ElementType::Int64:
        {
            result = LUE_ElementType_Int64;
            break;
        }
        case lue::ElementType::Float32:
        {
            result = LUE_ElementType_Float32;
            break;
        }
        case lue::ElementType::Float64:
        {
            result = LUE_ElementType_Float64;
            break;
        }
    }

    return result;
}


auto lue_element_type_array(LUE_Array const* array) -> LUE_ElementType
{
    auto scoped_enum = lue::api::element_type(lue_as_cxx_array(array));

    return element_type(scoped_enum);
}


auto lue_element_type_field(LUE_Field const* field) -> LUE_ElementType
{
    auto scoped_enum = lue::api::element_type(lue_as_cxx_field(field));

    return element_type(scoped_enum);
}


auto lue_element_type_literal(LUE_Literal const* literal) -> LUE_ElementType
{
    auto scoped_enum = lue::api::element_type(lue_as_cxx_literal(literal));

    return element_type(scoped_enum);
}


auto lue_element_type_scalar(LUE_Scalar const* scalar) -> LUE_ElementType
{
    auto scoped_enum = lue::api::element_type(lue_as_cxx_scalar(scalar));

    return element_type(scoped_enum);
}
