#include "hpx_unit_test.h"
#include "lue/framework/api/c/as_field.h"
#include "lue/framework/api/c/destruct.h"
#include "lue/framework/api/c/focal_operation.h"
#include "lue/framework/api/c/miscellaneous_operation.h"
// #include "lue/framework/api/c/type_info.h"


// TODO: All this assumes the used element types are supported. We need a C header with information about the
//       supported types. Picking the wrong type here results in link errors.


static void focal_mean_test()
{
    typedef float Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};
    LUE_Count const radius = 1;

    LUE_Field* field = NULL;

    {
        Element const value = 5.5;
        LUE_Literal* literal = lue_create_literal(value);
        LUE_Scalar* scalar = lue_create_scalar(literal);

        field = lue_as_field(lue_create_array(rank, array_shape, scalar));

        lue_destruct(scalar);
        lue_destruct(literal);
    }

    LUE_BooleanKernel* kernel = lue_create_box_kernel(radius);

    LUE_Field* result = lue_focal_mean(field, kernel);

    CU_ASSERT_NOT_EQUAL(result, NULL);
    CU_ASSERT_EQUAL(lue_data_model(result), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(result), LUE_ElementType_Float32);

    lue_destruct(result);
    lue_destruct(kernel);
    lue_destruct(field);
}


HPX_UNIT_TEST_SUITE("focal_operation", CUNIT_CI_TEST(focal_mean_test));
