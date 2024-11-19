import itertools

import lue.framework as lfr
import lue_test


def setUpModule():
    lue_test.start_hpx_runtime()


def tearDownModule():
    lue_test.stop_hpx_runtime()


class ZonalDiversityTest(lue_test.TestCase):
    @lue_test.framework_test_case
    def test_overloads(self):
        array_shape = (60, 40)
        fill_value = 5
        fill_zone = 3

        for value_element_type, zone_element_type in itertools.product(
            lfr.integral_element_types,
            lfr.zone_element_types,
        ):
            array = lfr.create_array(array_shape, value_element_type, fill_value)
            zones = lfr.create_array(array_shape, zone_element_type, fill_zone)
            _ = lfr.zonal_diversity(array, zones)
