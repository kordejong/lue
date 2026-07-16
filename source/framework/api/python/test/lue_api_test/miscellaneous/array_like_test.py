import lue.framework_x as lfrx
import lue_test
from lue_test.operation_test import setUpModule, tearDownModule


class ArrayLikeTest(lue_test.TestCase):
    @lue_test.framework_test_case
    def test_array_like(self):
        array_shape = (60, 40)
        partition_shape = (6, 4)

        for from_element_type in lfrx.arithmetic_element_types:
            from_scalar = lfrx.create_scalar(5, from_element_type)
            from_array = lfrx.create_array(
                array_shape, fill_value=from_scalar, partition_shape=partition_shape
            )

            for to_element_type in lfrx.arithmetic_element_types:
                to_scalar = lfrx.create_scalar(5, to_element_type)
                to_array = lfrx.array_like(from_array, fill_value=to_scalar)

                self.assertEqual(to_array.dtype, to_element_type)
                self.assertEqual(to_array.shape, array_shape)
