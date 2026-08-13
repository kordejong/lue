#include "hpx_unit_test.h"
#include "lue/framework/api/c/as_field.h"
#include "lue/framework/api/c/destruct.h"
#include "lue/framework/api/c/focal_operation.h"
#include "lue/framework/api/c/miscellaneous_operation.h"
// #include "lue/framework/api/c/type_info.h"


// TODO: All this assumes the used element types are supported. We need a C header with information about the
//       supported types. Picking the wrong type here results in link errors.


static void aspect_test(void)
{
    typedef float Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};

    LUE_Field* elevation = NULL;

    {
        Element const value = 5;
        LUE_Literal* literal = lue_create_literal(value);
        LUE_Scalar* scalar = lue_create_scalar(literal);

        elevation = lue_as_field(lue_create_array(rank, array_shape, scalar));

        lue_destruct(scalar);
        lue_destruct(literal);
    }

    LUE_Field* result = lue_aspect(elevation);

    CU_ASSERT_NOT_EQUAL(result, NULL);
    CU_ASSERT_EQUAL(lue_data_model(result), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(result), LUE_ElementType_Float32);

    lue_destruct(result);
    lue_destruct(elevation);
}


static void convolve_test(void)
{
    typedef float Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};
    LUE_Count const radius = 1;

    LUE_Field* field = NULL;

    {
        Element const value = 5;
        LUE_Literal* literal = lue_create_literal(value);
        LUE_Scalar* scalar = lue_create_scalar(literal);

        field = lue_as_field(lue_create_array(rank, array_shape, scalar));

        lue_destruct(scalar);
        lue_destruct(literal);
    }

    LUE_Kernel* kernel = NULL;

    {
        Element const value = 1;
        LUE_Literal* weight = lue_create_literal(value);

        kernel = lue_create_box_kernel(radius, weight);

        lue_destruct(weight);
    }

    LUE_Field* result = lue_convolve(field, kernel);

    CU_ASSERT_NOT_EQUAL(result, NULL);
    CU_ASSERT_EQUAL(lue_data_model(result), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(result), LUE_ElementType_Float32);

    lue_destruct(result);
    lue_destruct(kernel);
    lue_destruct(field);
}


static void focal_diversity_test(void)
{
    typedef int32_t Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};
    LUE_Count const radius = 1;

    LUE_Field* field = NULL;

    {
        Element const value = 5;
        LUE_Literal* literal = lue_create_literal(value);
        LUE_Scalar* scalar = lue_create_scalar(literal);

        field = lue_as_field(lue_create_array(rank, array_shape, scalar));

        lue_destruct(scalar);
        lue_destruct(literal);
    }

    LUE_Kernel* kernel = NULL;

    {
        LUE_BooleanElement const value = 1;
        LUE_Literal* weight = lue_create_literal(value);

        kernel = lue_create_box_kernel(radius, weight);

        lue_destruct(weight);
    }

    LUE_Field* result = lue_focal_diversity(field, kernel);

    CU_ASSERT_NOT_EQUAL(result, NULL);
    CU_ASSERT_EQUAL(lue_data_model(result), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(result), LUE_ElementType_Count);

    lue_destruct(result);
    lue_destruct(kernel);
    lue_destruct(field);
}


static void focal_high_pass_test(void)
{
    typedef float Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};
    LUE_Count const radius = 1;

    LUE_Field* field = NULL;

    {
        Element const value = 5.5F;
        LUE_Literal* literal = lue_create_literal(value);
        LUE_Scalar* scalar = lue_create_scalar(literal);

        field = lue_as_field(lue_create_array(rank, array_shape, scalar));

        lue_destruct(scalar);
        lue_destruct(literal);
    }

    LUE_Kernel* kernel = NULL;

    {
        LUE_BooleanElement const value = 1;
        LUE_Literal* weight = lue_create_literal(value);

        kernel = lue_create_box_kernel(radius, weight);

        lue_destruct(weight);
    }

    LUE_Field* result = lue_focal_high_pass(field, kernel);

    CU_ASSERT_NOT_EQUAL(result, NULL);
    CU_ASSERT_EQUAL(lue_data_model(result), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(result), LUE_ElementType_Float32);

    lue_destruct(result);
    lue_destruct(kernel);
    lue_destruct(field);
}


static void focal_majority_test(void)
{
    typedef int32_t Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};
    LUE_Count const radius = 1;

    LUE_Field* field = NULL;

    {
        Element const value = 5;
        LUE_Literal* literal = lue_create_literal(value);
        LUE_Scalar* scalar = lue_create_scalar(literal);

        field = lue_as_field(lue_create_array(rank, array_shape, scalar));

        lue_destruct(scalar);
        lue_destruct(literal);
    }

    LUE_Kernel* kernel = NULL;

    {
        LUE_BooleanElement const value = 1;
        LUE_Literal* weight = lue_create_literal(value);

        kernel = lue_create_box_kernel(radius, weight);

        lue_destruct(weight);
    }

    LUE_Field* result = lue_focal_majority(field, kernel);

    CU_ASSERT_NOT_EQUAL(result, NULL);
    CU_ASSERT_EQUAL(lue_data_model(result), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(result), LUE_ElementType_Int32);

    lue_destruct(result);
    lue_destruct(kernel);
    lue_destruct(field);
}


static void focal_maximum_test(void)
{
    typedef float Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};
    LUE_Count const radius = 1;

    LUE_Field* field = NULL;

    {
        Element const value = 5.5F;
        LUE_Literal* literal = lue_create_literal(value);
        LUE_Scalar* scalar = lue_create_scalar(literal);

        field = lue_as_field(lue_create_array(rank, array_shape, scalar));

        lue_destruct(scalar);
        lue_destruct(literal);
    }

    LUE_Kernel* kernel = NULL;

    {
        LUE_BooleanElement const value = 1;
        LUE_Literal* weight = lue_create_literal(value);

        kernel = lue_create_box_kernel(radius, weight);

        lue_destruct(weight);
    }

    LUE_Field* result = lue_focal_maximum(field, kernel);

    CU_ASSERT_NOT_EQUAL(result, NULL);
    CU_ASSERT_EQUAL(lue_data_model(result), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(result), LUE_ElementType_Float32);

    lue_destruct(result);
    lue_destruct(kernel);
    lue_destruct(field);
}


static void focal_mean_test(void)
{
    typedef float Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};
    LUE_Count const radius = 1;

    LUE_Field* field = NULL;

    {
        Element const value = 5.5F;
        LUE_Literal* literal = lue_create_literal(value);
        LUE_Scalar* scalar = lue_create_scalar(literal);

        field = lue_as_field(lue_create_array(rank, array_shape, scalar));

        lue_destruct(scalar);
        lue_destruct(literal);
    }

    LUE_Kernel* kernel = NULL;

    {
        LUE_BooleanElement const value = 1;
        LUE_Literal* weight = lue_create_literal(value);

        kernel = lue_create_box_kernel(radius, weight);

        lue_destruct(weight);
    }

    LUE_Field* result = lue_focal_mean(field, kernel);

    CU_ASSERT_NOT_EQUAL(result, NULL);
    CU_ASSERT_EQUAL(lue_data_model(result), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(result), LUE_ElementType_Float32);

    lue_destruct(result);
    lue_destruct(kernel);
    lue_destruct(field);
}


static void focal_minimum_test(void)
{
    typedef float Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};
    LUE_Count const radius = 1;

    LUE_Field* field = NULL;

    {
        Element const value = 5.5F;
        LUE_Literal* literal = lue_create_literal(value);
        LUE_Scalar* scalar = lue_create_scalar(literal);

        field = lue_as_field(lue_create_array(rank, array_shape, scalar));

        lue_destruct(scalar);
        lue_destruct(literal);
    }

    LUE_Kernel* kernel = NULL;

    {
        LUE_BooleanElement const value = 1;
        LUE_Literal* weight = lue_create_literal(value);

        kernel = lue_create_box_kernel(radius, weight);

        lue_destruct(weight);
    }

    LUE_Field* result = lue_focal_minimum(field, kernel);

    CU_ASSERT_NOT_EQUAL(result, NULL);
    CU_ASSERT_EQUAL(lue_data_model(result), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(result), LUE_ElementType_Float32);

    lue_destruct(result);
    lue_destruct(kernel);
    lue_destruct(field);
}


static void focal_sum_test(void)
{
    typedef float Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};
    LUE_Count const radius = 1;

    LUE_Field* field = NULL;

    {
        Element const value = 5.5F;
        LUE_Literal* literal = lue_create_literal(value);
        LUE_Scalar* scalar = lue_create_scalar(literal);

        field = lue_as_field(lue_create_array(rank, array_shape, scalar));

        lue_destruct(scalar);
        lue_destruct(literal);
    }

    LUE_Kernel* kernel = NULL;

    {
        LUE_BooleanElement const value = 1;
        LUE_Literal* weight = lue_create_literal(value);

        kernel = lue_create_box_kernel(radius, weight);

        lue_destruct(weight);
    }

    LUE_Field* result = lue_focal_sum(field, kernel);

    CU_ASSERT_NOT_EQUAL(result, NULL);
    CU_ASSERT_EQUAL(lue_data_model(result), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(result), LUE_ElementType_Float32);

    lue_destruct(result);
    lue_destruct(kernel);
    lue_destruct(field);
}


static void slope_test(void)
{
    typedef float Element;

    LUE_Rank const rank = 2;
    LUE_Count const array_shape[] = {60, 40};

    LUE_Field* elevation = NULL;

    {
        Element const value = 5;
        LUE_Literal* literal = lue_create_literal(value);
        LUE_Scalar* scalar = lue_create_scalar(literal);

        elevation = lue_as_field(lue_create_array(rank, array_shape, scalar));

        lue_destruct(scalar);
        lue_destruct(literal);
    }

    LUE_Literal* cell_size = NULL;

    {
        Element const value = 10;

        cell_size = lue_create_literal(value);
    }

    LUE_Field* result = lue_slope(elevation, cell_size);

    CU_ASSERT_NOT_EQUAL(result, NULL);
    CU_ASSERT_EQUAL(lue_data_model(result), LUE_DataModel_Array);
    CU_ASSERT_EQUAL(lue_element_type(result), LUE_ElementType_Float32);

    lue_destruct(result);
    lue_destruct(cell_size);
    lue_destruct(elevation);
}


HPX_UNIT_TEST_SUITE(
    "focal_operation",
    CUNIT_CI_TEST(aspect_test),
    CUNIT_CI_TEST(convolve_test),
    CUNIT_CI_TEST(focal_diversity_test),
    CUNIT_CI_TEST(focal_high_pass_test),
    CUNIT_CI_TEST(focal_majority_test),
    CUNIT_CI_TEST(focal_maximum_test),
    CUNIT_CI_TEST(focal_mean_test),
    CUNIT_CI_TEST(focal_minimum_test),
    CUNIT_CI_TEST(focal_sum_test),
    CUNIT_CI_TEST(slope_test));
