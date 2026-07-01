#!/usr/bin/env python
import sys
import unittest


if __name__ == "__main__":
    hpx_arguments = [
        argument for argument in sys.argv[1:] if argument.startswith("--hpx")
    ]

    unittest_arguments = [
        argument for argument in sys.argv if not argument.startswith("--hpx")
    ]

    unittest.main(module=None, argv=unittest_arguments)
