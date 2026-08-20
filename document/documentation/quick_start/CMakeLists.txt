add_executable(game_of_life
    game_of_life.cpp
)

target_link_libraries(game_of_life
    PRIVATE
        lue::framework_focal_operation
        lue::framework_local_operation
        lue::framework_miscellaneous_operation
        lue::framework_io
        lue::framework_model
        lue::gdal
        cxxopts::cxxopts
        HPX::wrap_main
)
