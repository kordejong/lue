add_test_conditionally(test)

configure_file(
    include/lue/netcdf4/configure.hpp.in
    include/lue/netcdf4/configure.hpp
)

configure_file(
    source/version.cpp.in
    source/version.cpp
)

add_library(lue_netcdf STATIC
    source/attribute.cpp
    source/dataset.cpp
    source/dimension.cpp
    source/error.cpp
    source/group.cpp
    source/type.cpp
    source/variable.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/source/version.cpp
)

add_library(lue::netcdf4 ALIAS lue_netcdf)

set_target_properties(lue_netcdf
    PROPERTIES
        EXPORT_NAME netcdf4
)

target_include_directories(lue_netcdf
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

target_sources(lue_netcdf
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
            ${CMAKE_CURRENT_BINARY_DIR}/include
        FILES
            include/lue
            ${CMAKE_CURRENT_BINARY_DIR}/include/lue/netcdf4/configure.hpp
)

target_link_libraries(lue_netcdf
    PUBLIC
        lue::shared
        netCDF::netcdf
)

lue_install_development_libraries(
    TARGETS lue_netcdf
)
