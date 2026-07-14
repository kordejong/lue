import lue.framework_x as lfrx
import lue_test
from lue_test.operation_test import setUpModule, tearDownModule


class CreateArrayTest(lue_test.TestCase):
    @lue_test.framework_test_case
    def test_create_array(self):
        array_shape = (60, 40)
        partition_shape = (6, 4)
        element_type = lfrx.signed_integral_element_types[0]

        for element_type in lfrx.arithmetic_element_types:
            scalar = lfrx.create_scalar(5, element_type)
            array = lfrx.create_array(
                array_shape, fill_value=scalar, partition_shape=partition_shape
            )
            self.assertEqual(array.dtype, element_type)
            self.assertEqual(array.shape, array_shape)

        # TODO Mirror c++ tests

        # self.assertTrue(False)

        # for input_type in [
        #     np.uint8,
        #     np.uint32,
        #     np.int32,
        #     np.uint64,
        #     np.int64,
        #     np.float32,
        #     np.float64,
        # ]:
        #     input_array = lfr.create_array(array_shape, input_type, fill_value)

        #     for output_type in [
        #         np.uint8,
        #         np.uint32,
        #         np.int32,
        #         np.uint64,
        #         np.int64,
        #         np.float32,
        #         np.float64,
        #     ]:
        #         _ = lfr.cast(input_array, output_type)


# BOOST_AUTO_TEST_CASE(add_raster_raster)
# {
#     using Element = std::int32_t;
#     using Raster = lue::PartitionedArray<Element, 2>;
#
#     lue::Field const raster1 = Raster{};
#     lue::Field const raster2 = Raster{};
#
#     lue::Field result = lue::add(raster1, raster2);
#
#     BOOST_CHECK(std::holds_alternative<Raster>(result));
# }
#
#
# BOOST_AUTO_TEST_CASE(add_raster_scalar)
# {
#     using Element = std::int32_t;
#     using Raster = lue::PartitionedArray<Element, 2>;
#     using Scalar = lue::Scalar<Element>;
#
#     lue::Field const raster = Raster{};
#     lue::Field const scalar = Scalar{};
#
#     lue::Field result = lue::add(raster, scalar);
#
#     BOOST_CHECK(std::holds_alternative<Raster>(result));
# }
#
#
# BOOST_AUTO_TEST_CASE(add_scalar_raster)
# {
#     using Element = std::int32_t;
#     using Scalar = lue::Scalar<Element>;
#     using Raster = lue::PartitionedArray<Element, 2>;
#
#     lue::Field const scalar = Scalar{};
#     lue::Field const raster = Raster{};
#
#     lue::Field result = lue::add(scalar, raster);
#
#     BOOST_CHECK(std::holds_alternative<Raster>(result));
# }
#
#
# BOOST_AUTO_TEST_CASE(add_scalar_scalar)
# {
#     using Element = std::int32_t;
#     using Scalar = lue::Scalar<Element>;
#
#     lue::Field const scalar1 = Scalar{};
#     lue::Field const scalar2 = Scalar{};
#
#     lue::Field result = lue::add(scalar1, scalar2);
#
#     BOOST_CHECK(std::holds_alternative<Scalar>(result));
# }
#
#
# BOOST_AUTO_TEST_CASE(add_raster_value)
# {
#     using Element = std::int32_t;
#     using Raster = lue::PartitionedArray<Element, 2>;
#
#     lue::Field const raster = Raster{};
#     lue::Field const value = Element{};
#
#     lue::Field result = lue::add(raster, value);
#
#     BOOST_CHECK(std::holds_alternative<Raster>(result));
# }
#
#
# BOOST_AUTO_TEST_CASE(add_value_raster)
# {
#     using Element = std::int32_t;
#     using Raster = lue::PartitionedArray<std::int32_t, 2>;
#
#     lue::Field const value = Element{};
#     lue::Field const raster = Raster{};
#
#     lue::Field result = lue::add(value, raster);
#
#     BOOST_CHECK(std::holds_alternative<Raster>(result));
# }
#
#
# BOOST_AUTO_TEST_CASE(add_scalar_value)
# {
#     using Element = std::int32_t;
#     using Scalar = lue::Scalar<Element>;
#
#     lue::Field const scalar = Scalar{};
#     lue::Field const value = Element{};
#
#     lue::Field result = lue::add(scalar, value);
#
#     BOOST_CHECK(std::holds_alternative<Scalar>(result));
# }
#
#
# BOOST_AUTO_TEST_CASE(add_value_scalar)
# {
#     using Element = std::int32_t;
#     using Scalar = lue::Scalar<Element>;
#
#     lue::Field const scalar = Scalar{};
#     lue::Field const value = Element{};
#
#     lue::Field result = lue::add(value, scalar);
#
#     BOOST_CHECK(std::holds_alternative<Scalar>(result));
# }
#
#
# BOOST_AUTO_TEST_CASE(add_value_value)
# {
#     using Element = std::int32_t;
#     using Scalar = lue::Scalar<Element>;
#
#     lue::Field const value1 = Element{};
#     lue::Field const value2 = Element{};
#
#     lue::Field result = lue::add(value1, value2);
#
#     BOOST_CHECK(std::holds_alternative<Scalar>(result));
# }
