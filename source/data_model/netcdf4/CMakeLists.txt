configure_file(
    include/lue/netcdf4/configure.hpp.in
    include/lue/netcdf4/configure.hpp
)

configure_file(
    source/version.cpp.in
    source/version.cpp
)

add_library(lue_netcdf4 SHARED
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

lue_configure_shared_library(
    TARGET_BASENAME netcdf4
    EXPORT_HEADER_PATHNAME include/lue/netcdf4/export.hpp
    EXPORT_MACRO_BASENAME NETCDF4
)

target_link_libraries(lue_netcdf4
    PUBLIC
        lue::shared
        netCDF::netcdf
)

if(LUE_BUILD_TESTS)
    add_subdirectory(test)
endif()
