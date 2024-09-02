from ..lue_test import OperationTest, setUpModule, tearDownModule, test_case


class AddTest(OperationTest):

    @test_case
    def test_add_array_array(self):
        for type_ in self.numeric_types:
            array = self.array[type_]
            _ = array + array
            array += array

    @test_case
    def test_add_array_scalar(self):
        for type_ in self.numeric_types:
            array = self.array[type_]
            scalar = self.array[type_]

            _ = array + scalar
            array += scalar

    @test_case
    def test_add_scalar_array(self):
        for type_ in self.numeric_types:
            array = self.array[type_]
            scalar = self.array[type_]

            _ = scalar + array
            scalar += array

    @test_case
    def test_add_scalar_scalar(self):
        for type_ in self.numeric_types:
            scalar = self.array[type_]

            _ = scalar + scalar
            scalar += scalar

    @test_case
    def test_add_array_value(self):
        # TODO This only works in specific cases, where the type of value equals the type of the elements in
        #      the array. Doc this in the Python-specific docs. Guideline: use Scalar.
        pass
        # for type_ in self.numeric_types:
        #     array = self.array[type_]
        #     value = self.value[type_]

        #     _ = array + value
        #     array += value

    @test_case
    def test_add_value_array(self):
        pass
        # for type_ in self.numeric_types:
        #     array = self.array[type_]
        #     value = self.value[type_]

        #     _ = value + array
        #     value += array

    @test_case
    def test_add_scalar_value(self):
        pass
        # for type_ in self.numeric_types:
        #     scalar = self.scalar[type_]
        #     value = self.value[type_]

        #     _ = scalar + value
        #     scalar += value

    @test_case
    def test_add_value_scalar(self):
        pass
        # for type_ in self.numeric_types:
        #     scalar = self.scalar[type_]
        #     value = self.value[type_]

        #     _ = value + scalar
        #     value += scalar
