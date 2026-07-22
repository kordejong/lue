import numpy as np

import lue.framework_x as lfrx
import lue_test
from lue_test.operation_test import OperationTest, setUpModule, tearDownModule


class ConvolveTest(OperationTest):
    @lue_test.framework_test_case
    def test_convolve(self):
        array_shape = (60, 40)

        for element_type in lfrx.floating_point_element_types:
            fill_value = lfrx.create_scalar(5, element_type)
            array = lfrx.create_array(array_shape, fill_value)
            field = lfrx.as_field(array)
            kernel = np.full((3, 3), 1, dtype=element_type)
            self.assert_overload(lfrx.convolve, field, kernel)
