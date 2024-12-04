set(names
    dataset
)

add_unit_tests(
    SCOPE lue_cf
    NAMES ${names}
    LIBRARIES
        lue::cf
    TARGETS
        test_names
)
