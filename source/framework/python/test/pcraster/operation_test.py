import numpy as np

import lue.framework as lfr
import lue.pcraster as lpr
import lue_test


def setUpModule():
    lue_test.start_hpx_runtime()


def tearDownModule():
    lue_test.stop_hpx_runtime()


class OperationTest(lue_test.TestCase):
    @classmethod
    @lue_test.framework_test_case
    def setUpClass(cls):
        cls.array_shape = (60, 40)
        cls.partition_shape = (10, 10)

        # TODO Can we do better?
        # - When the pcraster module must create spatials from non-spatials, this
        #   information currently must be passed in
        lpr.configuration.array_shape = cls.array_shape
        lpr.configuration.partition_shape = cls.partition_shape
        lpr.configuration.cell_size = 10

        cls.numeric_types = lpr.arithmetic_element_types

        cls.dtype_by_type = {
            type_: lue_test.dtype_by_type[type_] for type_ in cls.numeric_types
        }

        cls.value = {
            type_: lue_test.value_by_type[type_] for type_ in cls.numeric_types
        }
        cls.non_spatial = {
            type_: lfr.create_scalar(cls.dtype_by_type[type_], cls.value[type_])
            for type_ in cls.numeric_types
        }
        cls.spatial = {
            type_: lfr.create_array(
                cls.array_shape,
                type_,
                fill_value=value,
                partition_shape=cls.partition_shape,
            )
            for type_, value in cls.value.items()
        }

        # cls.non_spatial = {
        #     np.uint8: np.uint8(1),
        #     np.uint32: np.uint32(32),
        #     np.uint64: np.uint64(64),
        #     np.int32: np.int32(-32),
        #     np.int64: np.int64(-64),
        #     np.float32: np.float32(3.2),
        #     np.float64: np.float64(6.4),
        # }
        # cls.spatial = {
        #     np.uint8: lfr.create_array(
        #         cls.array_shape,
        #         np.uint8,
        #         fill_value=cls.non_spatial[np.uint8],
        #         partition_shape=cls.partition_shape,
        #     ),
        #     np.uint32: lfr.create_array(
        #         cls.array_shape,
        #         np.uint32,
        #         fill_value=cls.non_spatial[np.uint32],
        #         partition_shape=cls.partition_shape,
        #     ),
        #     np.uint64: lfr.create_array(
        #         cls.array_shape,
        #         np.uint64,
        #         fill_value=cls.non_spatial[np.uint64],
        #         partition_shape=cls.partition_shape,
        #     ),
        #     np.int32: lfr.create_array(
        #         cls.array_shape,
        #         np.int32,
        #         fill_value=cls.non_spatial[np.int32],
        #         partition_shape=cls.partition_shape,
        #     ),
        #     np.int64: lfr.create_array(
        #         cls.array_shape,
        #         np.int64,
        #         fill_value=cls.non_spatial[np.int64],
        #         partition_shape=cls.partition_shape,
        #     ),
        #     np.float32: lfr.create_array(
        #         cls.array_shape,
        #         np.float32,
        #         fill_value=cls.non_spatial[np.float32],
        #         partition_shape=cls.partition_shape,
        #     ),
        #     np.float64: lfr.create_array(
        #         cls.array_shape,
        #         np.float64,
        #         fill_value=cls.non_spatial[np.float64],
        #         partition_shape=cls.partition_shape,
        #     ),
        # }

        direction = 3

        cls.ldd = lfr.create_array(
            cls.array_shape,
            lpr.flow_direction_element_type,
            direction,
            partition_shape=cls.partition_shape,
        )
