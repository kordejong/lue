add_library(lue_cf SHARED)
add_library(lue::cf ALIAS lue_cf)

set_target_properties(lue_cf
    PROPERTIES
        EXPORT_NAME cf
        VERSION ${LUE_VERSION}
        SOVERSION ${LUE_VERSION_MAJOR}
)

generate_export_header(lue_cf
    EXPORT_FILE_NAME include/lue/cf/export.hpp
    EXPORT_MACRO_NAME LUE_CF_EXPORT
)

target_sources(lue_cf
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
            ${CMAKE_CURRENT_BINARY_DIR}/include
        FILES
            include/lue
            ${CMAKE_CURRENT_BINARY_DIR}/include/lue/cf/export.hpp
    PRIVATE
        # TODO remove?
        source/dataset.cpp
        # source/dimension.cpp
        # source/variable.cpp
        # /TODO

        source/netcdf/data_variable.cpp

        source/domain.cpp
        source/field.cpp

)

target_link_libraries(lue_cf
    PUBLIC
        lue::netcdf4
)

lue_install_runtime_libraries(
    TARGETS
        lue_cf
)

lue_install_development_libraries(
    TARGETS
        lue_cf
)

if(LUE_BUILD_TESTS)
    add_subdirectory(test)
endif()
