from .lue_test import OperationTest, setUpModule, tearDownModule, test_case


class IOTest(OperationTest):
    @test_case
    def test_from_gdal(self):
        name = "x"
        partition_shape = (10, 10)

        # TODO
        # lfrx.from_gdal(name)
        # lfrx.from_gdal(name, partition_shape)
