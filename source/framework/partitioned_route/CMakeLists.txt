# Instantiate serial route partition classes -----------------------------------
set(generated_source_files "")

block()
    set(offset "include/lue/framework/partitioned_route/server")
    set(output_pathname "${CMAKE_CURRENT_BINARY_DIR}/${offset}/serial_route_partition.hpp")

    set(Elements ${LUE_FRAMEWORK_ZONE_ELEMENTS})
    list(TRANSFORM Elements PREPEND "\"")
    list(TRANSFORM Elements APPEND "\"")
    list(JOIN Elements ", " Elements)

    generate_template_instantiation(
        INPUT_PATHNAME
            "${CMAKE_CURRENT_SOURCE_DIR}/${offset}/serial_route_partition.hpp.in"
        OUTPUT_PATHNAME
            "${output_pathname}"
        DICTIONARY
            '{"Elements":[${Elements}],"ranks":[${LUE_FRAMEWORK_RANKS}]}'
    )

    list(APPEND generated_source_files "${output_pathname}")

    foreach(RouteID IN LISTS LUE_FRAMEWORK_ZONE_ELEMENTS)
        string(REPLACE "::" "_" element ${RouteID})

        foreach(rank IN LISTS LUE_FRAMEWORK_RANKS)
            set(output_pathname "${CMAKE_CURRENT_BINARY_DIR}/source/serial_route_partition-${element}_${rank}.cpp")

            generate_template_instantiation(
                INPUT_PATHNAME
                    "${CMAKE_CURRENT_SOURCE_DIR}/source/serial_route_partition.cpp.in"
                OUTPUT_PATHNAME
                    "${output_pathname}"
                DICTIONARY
                    '{"RouteID":"${RouteID}","rank":"${rank}"}'
            )
            list(APPEND generated_source_files "${output_pathname}")

            set(output_pathname "${CMAKE_CURRENT_BINARY_DIR}/source/server/serial_route_partition-${element}_${rank}.cpp")

            generate_template_instantiation(
                INPUT_PATHNAME
                    "${CMAKE_CURRENT_SOURCE_DIR}/source/server/serial_route_partition.cpp.in"
                OUTPUT_PATHNAME
                    "${output_pathname}"
                DICTIONARY
                    '{"RouteID":"${RouteID}","rank":"${rank}"}'
            )
            list(APPEND generated_source_files "${output_pathname}")
        endforeach()
    endforeach()

    set(generated_source_files ${generated_source_files} PARENT_SCOPE)
endblock()


add_library(lue_framework_partitioned_route SHARED)
add_library(lue::framework_partitioned_route ALIAS lue_framework_partitioned_route)

set_target_properties(lue_framework_partitioned_route
    PROPERTIES
        EXPORT_NAME framework_partitioned_route
        VERSION ${LUE_VERSION}
        SOVERSION ${LUE_VERSION_MAJOR}
)

generate_export_header(lue_framework_partitioned_route
    EXPORT_FILE_NAME include/lue/framework/partitioned_route/export.hpp
    EXPORT_MACRO_NAME LUE_PARTITIONED_ROUTE_EXPORT
)

target_sources(lue_framework_partitioned_route
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
            ${CMAKE_CURRENT_BINARY_DIR}/include
        FILES
            include/lue
            ${CMAKE_CURRENT_BINARY_DIR}/include/lue/framework/partitioned_route/export.hpp
            ${CMAKE_CURRENT_BINARY_DIR}/include/lue/framework/partitioned_route/server/serial_route_partition.hpp
    PRIVATE
        source/server/serial_route_partition.cpp
        ${generated_source_files}
)

target_link_libraries(lue_framework_partitioned_route
    PRIVATE
        HPX::component
    PUBLIC
        lue::framework_core
)

lue_install_runtime_libraries(
    TARGETS
        lue_framework_partitioned_route
)

lue_install_development_libraries(
    TARGETS
        lue_framework_partitioned_route
)

if(LUE_BUILD_TESTS)
    add_subdirectory(test)
endif()
