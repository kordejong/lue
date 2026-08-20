import lue_test
from lue_test.operation_test import setUpModule, tearDownModule

import lue.framework_x as lfrx


class CellIndexTest(lue_test.TestCase):
    @lue_test.framework_test_case
    def test_cell_index(self):
        array_shape = (60, 40)
        partition_shape = (6, 4)
        condition_element_type = lfrx.boolean_element_type

        condition_scalar = lfrx.create_scalar(5, condition_element_type)
        condition_array = lfrx.create_array(
            array_shape, fill_value=condition_scalar, partition_shape=partition_shape
        )
        condition_field = lfrx.as_field(condition_array)

        result = lfrx.cell_index(condition_field, 0)

        self.assertEqual(result.data_model, lfrx.DataModel.Array)
        self.assertEqual(result.dtype, lfrx.index_element_type)
