import numpy as np

import lue.framework as lfr
import lue_test


def setUpModule():
    lue_test.start_hpx_runtime()


def tearDownModule():
    lue_test.stop_hpx_runtime()


class UniformTest(lue_test.TestCase):
    @lue_test.framework_test_case
    def test_overload1(self):
        array_shape = (60, 40)
        min_value = 33
        max_value = 55
        fill_value = 5

        for element_type in lfr.arithmetic_element_types:
            array = lfr.create_array(array_shape, element_type, fill_value)

            for result_element_type in lfr.arithmetic_element_types:
                if result_element_type not in [np.uint8, np.int8]:
                    _ = lfr.uniform(array, result_element_type, min_value, max_value)

    @lue_test.framework_test_case
    def test_overload2(self):
        array_shape = (60, 40)
        min_value = 33
        max_value = 55

        for element_type in lfr.arithmetic_element_types:
            if element_type not in [np.uint8, np.int8]:
                _ = lfr.uniform(array_shape, element_type, min_value, max_value)
