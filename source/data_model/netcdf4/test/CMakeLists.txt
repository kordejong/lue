set(names
    attribute
    dataset
    dimension
    group
    variable

    use_case/weather_forecast
)

add_unit_tests(
    SCOPE lue_netcdf4
    NAMES ${names}
    LIBRARIES
        lue_netcdf4_static
    TARGETS
        test_names
)
