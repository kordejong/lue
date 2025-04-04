add_library(lue_netcdf4 SHARED)
add_library(lue::netcdf4 ALIAS lue_netcdf4)

set_target_properties(lue_netcdf4
    PROPERTIES
        EXPORT_NAME netcdf4
        VERSION ${LUE_VERSION}
        SOVERSION ${LUE_VERSION_MAJOR}
)

generate_export_header(lue_netcdf4
    EXPORT_FILE_NAME include/lue/netcdf4/export.hpp
    EXPORT_MACRO_NAME LUE_NETCDF4_EXPORT
)

configure_file(
    include/lue/netcdf4/configure.hpp.in
    include/lue/netcdf4/configure.hpp
)

configure_file(
    source/version.cpp.in
    source/version.cpp
)

target_sources(lue_netcdf4
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
            ${CMAKE_CURRENT_BINARY_DIR}/include
        FILES
            include/lue
            ${CMAKE_CURRENT_BINARY_DIR}/include/lue/netcdf4/configure.hpp
            ${CMAKE_CURRENT_BINARY_DIR}/include/lue/netcdf4/export.hpp
    PRIVATE
        source/attribute.cpp
        source/dataset.cpp
        source/dimension.cpp
        source/error.cpp
        source/group.cpp
        source/hyperslab.cpp
        source/type.cpp
        source/variable.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/source/version.cpp
)

target_link_libraries(lue_netcdf4
    PUBLIC
        lue::shared
        netCDF::netcdf
)

lue_install_runtime_libraries(
    TARGETS
        lue_netcdf4
)

lue_install_development_libraries(
    TARGETS
        lue_netcdf4
)

if(LUE_BUILD_TESTS)
    lue_configure_static_library_for_tests(
        TARGET_NAME_SHARED_LIB lue_netcdf4
        TARGET_NAME_STATIC_LIB lue_netcdf4_static
        EXPORT_MACRO_BASENAME NETCDF4
    )

    add_subdirectory(test)
endif()
