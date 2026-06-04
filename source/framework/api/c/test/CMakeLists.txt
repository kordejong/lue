set(scope lue_framework_api_c)

set(names
    io
)

foreach(name ${names})
    set(module_name ${name}_test)
    string(REPLACE "/" "_" test_name ${module_name})
    set(test_name ${scope}_${test_name})

    add_executable(${test_name} ${module_name}.cpp)

    target_include_directories(${test_name}
        PRIVATE
            $<BUILD_INTERFACE:$<TARGET_PROPERTY:lue::framework_api_c,SOURCE_DIR>/source>  # field.hpp
            $<BUILD_INTERFACE:$<TARGET_PROPERTY:lue::framework_api_cxx,SOURCE_DIR>/test>  # unit_test.hpp
    )
    target_link_libraries(${test_name}
        PRIVATE
            lue::framework_api_c
            lue::framework_api_cxx
            lue::framework_test
            Boost::headers
    )

    add_test(NAME ${test_name}
        COMMAND
            ${Python_EXECUTABLE} ${HPXRUN}
                "--runwrapper" ${LUE_QUALITY_ASSURANCE_TEST_HPX_RUNWRAPPER}
                "--parcelport" ${LUE_QUALITY_ASSURANCE_TEST_HPX_PARCELPORT}
                "--localities" ${LUE_QUALITY_ASSURANCE_TEST_NR_LOCALITIES_PER_TEST}
                "--thread" ${LUE_QUALITY_ASSURANCE_TEST_NR_THREADS_PER_LOCALITY} --
                    "$<TARGET_FILE:${test_name}>"
    )

    set_property(
        TEST
            ${test_name}
        APPEND
        PROPERTY
            ENVIRONMENT_MODIFICATION
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_api_cxx>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_partitioned_array>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_partitioned_route>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_local_operation>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_focal_operation>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_routing_operation>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_zonal_operation>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::gdal>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:HPX::component_storage_component>>
    )
endforeach()


# add_library(lue_framework_api_c_unit_test
#     unit_test.cpp
# )
#
# target_link_libraries(lue_framework_api_c_unit_test
#     PRIVATE
#         lue::framework_api_cxx
#         lue::shared
# )
#
# target_include_directories(lue_framework_api_c_unit_test
#     PRIVATE
#         # $<BUILD_INTERFACE:$<TARGET_PROPERTY:lue::framework_api_c,SOURCE_DIR>/source>  # field.hpp
#         $<BUILD_INTERFACE:$<TARGET_PROPERTY:lue::framework_api_cxx,SOURCE_DIR>/test>  # unit_test.hpp
# )

set(names
    local_operation
    miscellaneous_operation
)

foreach(name ${names})
    set(module_name ${name}_test)
    string(REPLACE "/" "_" test_name ${module_name})
    set(test_name ${scope}_${test_name})

    add_executable(${test_name} ${module_name}.c)

    target_link_libraries(${test_name}
        PRIVATE
            lue::framework_api_c
            # lue_framework_api_c_unit_test
            cunit
    )

    add_test(NAME ${test_name}
        COMMAND
            ${Python_EXECUTABLE} ${HPXRUN}
                "--runwrapper" ${LUE_QUALITY_ASSURANCE_TEST_HPX_RUNWRAPPER}
                "--parcelport" ${LUE_QUALITY_ASSURANCE_TEST_HPX_PARCELPORT}
                "--localities" ${LUE_QUALITY_ASSURANCE_TEST_NR_LOCALITIES_PER_TEST}
                "--thread" ${LUE_QUALITY_ASSURANCE_TEST_NR_THREADS_PER_LOCALITY} --
                    "$<TARGET_FILE:${test_name}>"
    )

    set_property(
        TEST
            ${test_name}
        APPEND
        PROPERTY
            ENVIRONMENT_MODIFICATION
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_api_cxx>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_partitioned_array>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_partitioned_route>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_local_operation>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_focal_operation>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_routing_operation>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::framework_zonal_operation>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:lue::gdal>>
                PATH=path_list_prepend:$<$<PLATFORM_ID:Windows>:$<TARGET_FILE_DIR:HPX::component_storage_component>>
    )
endforeach()
