add_library(lue_document SHARED)
add_library(lue::document ALIAS lue_document)

set_target_properties(lue_document
    PROPERTIES
        EXPORT_NAME document
        VERSION ${LUE_VERSION}
        SOVERSION ${LUE_VERSION_MAJOR}
)

generate_export_header(lue_document
    EXPORT_FILE_NAME include/lue/document/export.hpp
    EXPORT_MACRO_NAME LUE_DOCUMENT_EXPORT
)

target_sources(lue_document
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
            ${CMAKE_CURRENT_BINARY_DIR}/include
        FILES
            include/lue
            ${CMAKE_CURRENT_BINARY_DIR}/include/lue/document/export.hpp
    PRIVATE
        source/example.cpp
)

target_link_libraries(lue_document
    PUBLIC
        lue::framework_algorithm
        lue::framework_io
        lue::gdal
        lue::utility
)

if(LUE_BUILD_TESTS)
    lue_configure_static_library_for_tests(
        TARGET_NAME_SHARED_LIB lue_document
        TARGET_NAME_STATIC_LIB lue_document_static
        EXPORT_MACRO_BASENAME DOCUMENT
    )

    add_subdirectory(test)
endif()
