#include "lue/framework/api/c/miscellaneous_operation.h"
#include "literal.hpp"
#include "scalar.hpp"
#include "lue/framework/api/cxx/miscellaneous_operation.hpp"


auto lue_create_array(LUE_Rank const rank, LUE_Count const* array_shape_p, LUE_Scalar const* fill_value) -> LUE_Array*
{
    assert(rank == 2);

    lue::Shape<lue::Count, 2> array_shape{};
    std::copy(array_shape_p, array_shape_p + rank, array_shape.data());

    lue::api::Array result = lue::api::create_array(array_shape, lue_as_cxx_scalar(fill_value));

    return new LUE_Array{.instance = new lue::api::Array{std::move(result)}};
}


#define CreateLiteral(type, type_name)                                                                       \
    auto lue_create_literal_##type_name(type const value) -> LUE_Literal*                                                  \
    {                                                                                                        \
        lue::api::Literal result{value};                                                                     \
                                                                                                             \
        return new LUE_Literal{.instance = new lue::api::Literal{std::move(result)}};                            \
    }

CreateLiteral(uint8_t, uint8)
CreateLiteral(uint16_t, uint16)
CreateLiteral(uint32_t, uint32)
CreateLiteral(uint64_t, uint64)
CreateLiteral(int8_t, int8)
CreateLiteral(int16_t, int16)
CreateLiteral(int32_t, int32)
CreateLiteral(int64_t, int64)
CreateLiteral(float, float32)
CreateLiteral(double, float64)

#undef CreateLiteral


    auto lue_create_scalar(LUE_Literal* fill_value) -> LUE_Scalar*
{
    lue::api::Scalar result = lue::api::create_scalar(lue_as_cxx_literal(fill_value));

    return new LUE_Scalar{.instance = new lue::api::Scalar{std::move(result)}};
}
