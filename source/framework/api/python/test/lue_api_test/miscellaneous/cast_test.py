import lue.framework_x as lfrx
import lue_test
from lue_test.operation_test import setUpModule, tearDownModule


class CastTest(lue_test.TestCase):
    @lue_test.framework_test_case
    def test_cast(self):
        array_shape = (60, 40)
        partition_shape = (6, 4)

        for from_element_type in lfrx.arithmetic_element_types:
            from_scalar = lfrx.create_scalar(5, from_element_type)
            from_array = lfrx.create_array(
                array_shape, fill_value=from_scalar, partition_shape=partition_shape
            )
            from_field = lfrx.as_field(from_array)

            for to_element_type in lfrx.arithmetic_element_types:
                to_field = lfrx.cast(from_field, to_element_type)

                self.assertEqual(to_field.data_model, lfrx.DataModel.Array)
                self.assertEqual(to_field.dtype, to_element_type)
