import unittest

import lue.framework_x as lfrx


def test_case(test_case):
    def wrapper(self):
        if lfrx.on_root_locality():
            test_case(self)

    return wrapper


def setUpModule():
    lfrx.start_hpx_runtime(lfrx.default_hpx_runtime_configuration)


def tearDownModule():
    # TODO start/stop/start/stop/... doesn't work yet. Would be nice if it did, but in general being able to
    # start and stop once is sufficient.
    # lfrx.stop_hpx_runtime()

    pass


class TestCase(unittest.TestCase):
    def setUp(self):
        pass

    def tearDown(self):
        pass
