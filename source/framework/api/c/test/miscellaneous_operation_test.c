#include "hpx_unit_test.h"
#include "lue/framework/api/c/miscellaneous_operation.h"


static void create_array_test()
{
    // TODO: This assumes float32 is supported. We need a C header with information about the supported
    //       types. Picking the wrong type here results in link errors.
    typedef float Element;

    Rank const rank = 2;
    Count const array_shape[] = {60, 40};

    Element const value = 5.5;
    Field* fill_value = create_literal(value);
    Field* result = create_array(rank, array_shape, fill_value);

    CU_ASSERT_NOT_EQUAL(result, NULL);

    // TODO: How to easily check the type of the concrete type?

    destruct_field(result);
    destruct_field(fill_value);
}


HPX_UNIT_TEST_SUITE("miscellaneous_operation", CUNIT_CI_TEST(create_array_test));
