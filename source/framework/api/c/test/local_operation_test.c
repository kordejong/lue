#include "hpx_unit_test.h"
#include "lue/framework/api/c/as_field.h"
#include "lue/framework/api/c/destruct.h"
#include "lue/framework/api/c/local_operation.h"
#include "lue/framework/api/c/miscellaneous_operation.h"
#include "lue/framework/api/c/type_info.h"


// TODO: All this assumes the used element types are supported. We need a C header with information about the
//       supported types. Picking the wrong type here results in link errors.


static void abs_test()
{
    typedef int32_t Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};

    Element const value = -5;
    LUE_Literal* literal = lue_create_literal(value);
    LUE_Scalar* scalar = lue_create_scalar(literal);
    LUE_Field* field = lue_as_field(lue_create_array(rank, array_shape, scalar));

    LUE_Field* result = lue_abs(field);

    CU_ASSERT_NOT_EQUAL(result, NULL);
    CU_ASSERT_EQUAL(lue_data_model(result), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(result), LUE_ElementType_Int32);

    lue_destruct(result);
    lue_destruct(field);
    lue_destruct(scalar);
    lue_destruct(literal);
}


HPX_UNIT_TEST_SUITE("local_operation", CUNIT_CI_TEST(abs_test));


// TODO:
// abs
// acos
// add
// asin
// atan
// atan2
// ceil
// close_to
// cos
// divide
// equal_to
// exp
// floor
// greater_than
// greater_than_equal_to
// less_than
// less_than_equal_to
// log
// log10
// logical_and
// logical_exclusive_or
// logical_inclusive_or
// logical_not
// modulus
// multiply
// negate
// not_equal_to
// open_simplex_noise
// pow
// round
// sin
// sqrt
// subtract
// tan
// trunc
// uniform
// valid
// where2
// where3
