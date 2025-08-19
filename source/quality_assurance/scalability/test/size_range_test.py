import unittest

from lue.command.scalability.experiment.size_range import SizeRange


class SizeRangeTest(unittest.TestCase):
    def _test_case_multiplier(self, min_size, max_size, multiplier, permutation_sizes):
        data = {
            "min_size": min_size,
            "max_size": max_size,
            "multiplier": multiplier,
        }

        size_range = SizeRange.from_data(data)

        self.assertEqual(size_range.nr_sizes, len(permutation_sizes))
        self.assertEqual(size_range.range.min_size, permutation_sizes[0])
        self.assertEqual(size_range.range.max_size, permutation_sizes[-1])

        for i in range(len(permutation_sizes)):
            self.assertEqual(size_range.size(i), permutation_sizes[i])

    def test_case_1_64_2_multiplier(self):
        self._test_case_multiplier(1, 64, 2, [1, 2, 4, 8, 16, 32, 64])

    def test_case_1_2_2_multiplier(self):
        self._test_case_multiplier(1, 2, 2, [1, 2])

    def test_case_1_3_2_multiplier(self):
        self._test_case_multiplier(1, 3, 2, [1, 2])

    def test_case_4_35_3_multiplier(self):
        self._test_case_multiplier(4, 35, 3, [4, 12])

    def test_case_4_36_3_multiplier(self):
        self._test_case_multiplier(4, 36, 3, [4, 12, 36])

    def test_case_4_37_3_multiplier(self):
        self._test_case_multiplier(4, 37, 3, [4, 12, 36])

    def _test_case_incrementor(
        self, min_size, max_size, incrementor, permutation_sizes
    ):
        data = {
            "min_size": min_size,
            "max_size": max_size,
            "incrementor": incrementor,
        }

        size_range = SizeRange.from_data(data)

        self.assertEqual(size_range.nr_sizes, len(permutation_sizes))
        self.assertEqual(size_range.min_size, permutation_sizes[0])
        self.assertEqual(size_range.max_size, permutation_sizes[-1])

        for i in range(len(permutation_sizes)):
            self.assertEqual(size_range.size(i), permutation_sizes[i])

    def test_case_1_5_1_incrementor(self):
        self._test_case_incrementor(1, 5, 1, [1, 2, 3, 4, 5])

    def test_case_1_5_2_incrementor(self):
        self._test_case_incrementor(1, 5, 2, [1, 3, 5])

    def test_case_1_4_2_incrementor(self):
        self._test_case_incrementor(1, 4, 2, [1, 3])

    def test_case_1_6_2_incrementor(self):
        self._test_case_incrementor(1, 6, 2, [1, 3, 5])
