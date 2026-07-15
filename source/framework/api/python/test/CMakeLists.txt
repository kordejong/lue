add_test(
    NAME lue_framework_api_python_test
    COMMAND
        ${Python_EXECUTABLE} ${HPXRUN}
            "--runwrapper" ${LUE_QUALITY_ASSURANCE_TEST_HPX_RUNWRAPPER}
            "--parcelport" ${LUE_QUALITY_ASSURANCE_TEST_HPX_PARCELPORT}
            "--localities" ${LUE_QUALITY_ASSURANCE_TEST_NR_LOCALITIES_PER_TEST}
            "--thread" ${LUE_QUALITY_ASSURANCE_TEST_NR_THREADS_PER_LOCALITY} --
                ${Python_EXECUTABLE}
                    -m lue_unit_test discover
                    --verbose
                    --start-directory ${CMAKE_CURRENT_SOURCE_DIR}
                    --pattern "*_test.py"
)

lue_configure_python_test(
    TARGET
        lue_framework_api_python_test
)

set_property(
    TEST
        lue_framework_api_python_test
    APPEND
    PROPERTY
        ENVIRONMENT_MODIFICATION
            PYTHONPATH=path_list_prepend:$<TARGET_PROPERTY:lue::py_framework,SOURCE_DIR>/test
            PYTHONPATH=path_list_prepend:${CMAKE_CURRENT_SOURCE_DIR}
)
