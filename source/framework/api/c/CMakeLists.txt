add_library(lue_framework_api_c SHARED)
add_library(lue::framework_api_c ALIAS lue_framework_api_c)

set_target_properties(lue_framework_api_c
    PROPERTIES
        EXPORT_NAME framework_api_c
        VERSION ${LUE_VERSION}
        SOVERSION ${LUE_VERSION_MAJOR}
)

generate_export_header(lue_framework_api_c
    EXPORT_FILE_NAME include/lue/framework/api/c/export.h
    EXPORT_MACRO_NAME LUE_FCAPI_EXPORT
)

target_sources(lue_framework_api_c
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
            ${CMAKE_CURRENT_BINARY_DIR}/include
        FILES
            include/lue
            ${CMAKE_CURRENT_BINARY_DIR}/include/lue/framework/api/c/export.h
    PRIVATE
        source/array.cpp
        source/as_field.cpp
        source/field.cpp
        source/future.cpp
        source/io.cpp
        source/literal.cpp
        source/local_operation.cpp
        source/miscellaneous_operation.cpp
        source/runtime.cpp
        source/scalar.cpp
        source/type_info.cpp
)

# set_property(TARGET lue_framework_api_c
#     PROPERTY C_STANDARD 11
# )

target_link_libraries(lue_framework_api_c
    PRIVATE
        lue::framework_api_cxx
)

lue_install_runtime_libraries(
    TARGETS
        lue_framework_api_c
)

lue_install_development_libraries(
    TARGETS
        lue_framework_api_c
)

if(LUE_BUILD_TESTS)
    add_subdirectory(test)
endif()
