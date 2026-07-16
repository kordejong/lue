import lue.framework_x as lfrx
import lue_test
from lue_test.operation_test import setUpModule, tearDownModule


class CreateScalarTest(lue_test.TestCase):
    @lue_test.framework_test_case
    def test_create_scalar(self):
        for element_type in lfrx.arithmetic_element_types:
            scalar = lfrx.create_scalar(5, element_type)
            self.assertEqual(scalar.dtype, element_type)
