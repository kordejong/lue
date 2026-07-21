import numpy as np

import lue.framework_x as lfrx
import lue_test
from lue_test.operation_test import OperationTest, setUpModule, tearDownModule


class AspectTest(OperationTest):
    @lue_test.framework_test_case
    def test_aspect(self):
        array_shape = (60, 40)

        for element_type in lfrx.floating_point_element_types:
            fill_value = lfrx.create_scalar(5, element_type)
            array = lfrx.create_array(array_shape, fill_value)
            field = lfrx.as_field(array)
            self.assert_overload(lfrx.aspect, field)
