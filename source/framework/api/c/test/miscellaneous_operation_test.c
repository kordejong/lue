#include "hpx_unit_test.h"
#include "lue/framework/api/c/miscellaneous_operation.h"
#include "lue/framework/api/c/type_info.h"


// TODO: All this assumes the used element types are supported. We need a C header with information about the
//       supported types. Picking the wrong type here results in link errors.


static void create_array_test()
{
    typedef float Element;

    Rank const rank = 2;
    Count const array_shape[] = {60, 40};

    Element const value = 5.5F;
    Literal* literal = create_literal(value);
    Scalar* scalar = create_scalar(literal);
    Field* array = create_array(rank, array_shape, scalar);

    CU_ASSERT_NOT_EQUAL(array, NULL);
    CU_ASSERT_EQUAL(lue_data_model(array), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(array), LUE_ElementType_Float32);

    destruct_field(array);
    destruct_scalar(scalar);
    destruct_literal(literal);
}


static void create_literal_test()
{
    typedef uint8_t Element;

    Element const value = 5;
    Literal* literal = create_literal(value);

    CU_ASSERT_NOT_EQUAL(literal, NULL);
    CU_ASSERT_EQUAL(lue_element_type(literal), LUE_ElementType_UInt8);

    destruct_literal(literal);
}


static void create_scalar_test()
{
    typedef int32_t Element;

    Element const value = -5;
    Literal* literal = create_literal(value);
    Scalar* scalar = create_scalar(literal);

    CU_ASSERT_NOT_EQUAL(scalar, NULL);
    CU_ASSERT_EQUAL(lue_element_type(scalar), LUE_ElementType_Int32);

    destruct_scalar(scalar);
    destruct_literal(literal);
}


HPX_UNIT_TEST_SUITE(
    "miscellaneous_operation",
    CUNIT_CI_TEST(create_array_test),
    CUNIT_CI_TEST(create_literal_test),
    CUNIT_CI_TEST(create_scalar_test));
