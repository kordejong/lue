import numpy as np

import lue.framework_x as lfrx

from ..lue_test import OperationTest, setUpModule, tearDownModule, test_case


class UniformTest(OperationTest):

    @test_case
    def test_uniform(self):
        for type_ in self.numeric_types:
            if type_ != np.uint8:
                min_ = self.scalar[type_]
                max_ = self.scalar[type_]
                _ = lfrx.uniform(self.array_shape, self.partition_shape, min_, max_)
                _ = lfrx.uniform(self.array_shape, min_, max_)
