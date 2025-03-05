set(names
    # TODO remove?
    # dataset
    # /TODO
)

add_unit_tests(
    SCOPE lue_cf
    NAMES ${names}
    LIBRARIES
        lue::cf
    TARGETS
        test_names
)
