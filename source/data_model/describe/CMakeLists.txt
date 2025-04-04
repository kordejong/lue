add_executable(lue_describe
    source/describe.cpp
    source/describe_netcdf4.cpp
    source/describe_cf.cpp
    source/main.cpp
)

target_include_directories(lue_describe
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(lue_describe
    PRIVATE
        lue::utility
        lue::cf
)

lue_install_executables(
    TARGETS lue_describe
)
