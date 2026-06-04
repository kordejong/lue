#include "hpx_unit_test.h"
#include "lue/framework/api/c/destruct.h"
#include "lue/framework/api/c/miscellaneous_operation.h"
#include "lue/framework/api/c/type_info.h"


// TODO: All this assumes the used element types are supported. We need a C header with information about the
//       supported types. Picking the wrong type here results in link errors.


static void create_array_test()
{
    typedef float Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};

    Element const value = 5.5F;
    LUE_Literal* literal = lue_create_literal(value);
    LUE_Scalar* scalar = lue_create_scalar(literal);
    LUE_Array* array = lue_create_array(rank, array_shape, scalar);

    CU_ASSERT_NOT_EQUAL(array, NULL);
    CU_ASSERT_EQUAL(lue_element_type(array), LUE_ElementType_Float32);

    lue_destruct(array);
    lue_destruct(scalar);
    lue_destruct(literal);
}


static void create_literal_test()
{
    typedef uint8_t Element;

    Element const value = 5;
    LUE_Literal* literal = lue_create_literal(value);

    CU_ASSERT_NOT_EQUAL(literal, NULL);
    CU_ASSERT_EQUAL(lue_element_type(literal), LUE_ElementType_UInt8);

    lue_destruct(literal);
}


static void create_scalar_test()
{
    typedef int32_t Element;

    Element const value = -5;
    LUE_Literal* literal = lue_create_literal(value);
    LUE_Scalar* scalar = lue_create_scalar(literal);

    CU_ASSERT_NOT_EQUAL(scalar, NULL);
    CU_ASSERT_EQUAL(lue_element_type(scalar), LUE_ElementType_Int32);

    lue_destruct(scalar);
    lue_destruct(literal);
}


HPX_UNIT_TEST_SUITE(
    "miscellaneous_operation",
    CUNIT_CI_TEST(create_array_test),
    CUNIT_CI_TEST(create_literal_test),
    CUNIT_CI_TEST(create_scalar_test));
