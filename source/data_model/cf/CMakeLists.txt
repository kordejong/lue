add_test_conditionally(test)

add_library(lue_cf STATIC
    source/dataset.cpp
    source/dimension.cpp
    source/domain.cpp
    source/field.cpp
    source/group.cpp
    source/variable.cpp
)

add_library(lue::cf ALIAS lue_cf)

set_target_properties(lue_cf
    PROPERTIES
        EXPORT_NAME cf
)

target_include_directories(lue_cf
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_sources(lue_cf
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
            ${CMAKE_CURRENT_BINARY_DIR}/include
        FILES
            include/lue
)

target_link_libraries(lue_cf
    PUBLIC
        lue::netcdf4
)

lue_install_development_libraries(
    TARGETS lue_cf
)
