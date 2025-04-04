set(names
    # TODO remove?
    # dataset
    # /TODO
)

add_unit_tests(
    SCOPE lue_cf
    NAMES ${names}
    LIBRARIES
        lue_cf_static
    TARGETS
        test_names
)
