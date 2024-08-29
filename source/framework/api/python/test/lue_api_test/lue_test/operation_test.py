import lue.framework_x as lfrx

from . import type as type_
from .test_case import TestCase, test_case


def setUpModule():
    pass
    # lue_test.start_hpx_runtime()


def tearDownModule():
    pass
    # lue_test.stop_hpx_runtime()


class OperationTest(TestCase):
    @classmethod
    @test_case
    def setUpClass(cls):
        cls.boolean_types = type_.boolean_types
        cls.unsigned_integral_types = type_.unsigned_integral_types
        cls.signed_integral_types = type_.signed_integral_types
        cls.integral_types = type_.integral_types
        cls.floating_point_types = type_.floating_point_types
        cls.numeric_types = type_.numeric_types
        cls.value = type_.value_by_type

        cls.array_shape = (60, 40)
        cls.partition_shape = (10, 10)

        cls.array = {
            # type_: lfrx.create_array(
            #     cls.array_shape,
            #     type_,
            #     fill_value=value,
            #     partition_shape=cls.partition_shape,
            # )
            # for type_, value in cls.value.items()
        }

        # cls.scalar = {type_: lfrx.maximum(array) for type_, array in cls.array.items()}

        cls.dtype_by_type = type_.dtype_by_type

        cls.scalar = {
            # type_: lfrx.create_scalar(cls.dtype_by_type[type_], cls.value[type_])
            # for type_ in cls.numeric_types
        }
