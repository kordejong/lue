add_library(lue_cf SHARED
    # TODO remove?
    source/dataset.cpp
    # source/dimension.cpp
    # source/group.cpp
    # source/variable.cpp
    # /TODO

    source/netcdf/data_variable.cpp

    source/domain.cpp
    source/field.cpp
)

lue_configure_shared_library(
    TARGET_BASENAME cf
    EXPORT_HEADER_PATHNAME include/lue/cf/export.hpp
    EXPORT_MACRO_BASENAME CF
)

target_link_libraries(lue_cf
    PUBLIC
        lue::netcdf4
)

if(LUE_BUILD_TESTS)
    add_subdirectory(test)
endif()
