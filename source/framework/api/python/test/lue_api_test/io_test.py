import lue.framework_x as lfrx

from .lue_test import OperationTest, setUpModule, tearDownModule, test_case


class IOTest(OperationTest):
    @test_case
    def test_from_gdal(self):

        for type_ in self.numeric_types:
            name = f"api_python_io_round_trip.tif"

            lfrx.to_gdal(self.array[type_], name).wait()
            _ = lfrx.from_gdal(name)
