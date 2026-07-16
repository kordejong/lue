# Stress

Scripts that perform "stress tests". These tests are not run as part of the test target (their names don't
match the pattern used by Python's `unittest` module. They potentially run for a long time and can be used to
check for crashes and memory usage.
