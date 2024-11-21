set(names
    attribute
    dataset
    dimension
    group
    variable
)

add_unit_tests(
    SCOPE lue_netcdf
    NAMES ${names}
    LIBRARIES
        lue::netcdf4
    TARGETS
        test_names
)
