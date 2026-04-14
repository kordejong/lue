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

set_property(
    TEST
        lue_framework_api_python_test
    APPEND
    PROPERTY
        ENVIRONMENT_MODIFICATION
            # Preload the custom allocator shared library, if necessary
            LD_PRELOAD=path_list_append:$<$<AND:$<PLATFORM_ID:Linux>,$<BOOL:${Jemalloc_LIBRARY}>>:${Jemalloc_LIBRARY}>
            LD_PRELOAD=path_list_append:$<$<AND:$<PLATFORM_ID:Linux>,$<BOOL:${Tcmalloc_LIBRARY}>>:${Tcmalloc_LIBRARY}>

            # Add the path to our Python modules to PYTHONPATH.
            PYTHONPATH=path_list_prepend:$<TARGET_FILE_DIR:lue::py>/..
            PYTHONPATH=path_list_prepend:$<TARGET_PROPERTY:lue::py_framework,SOURCE_DIR>/test
            PYTHONPATH=path_list_prepend:${CMAKE_CURRENT_SOURCE_DIR}
            # No need to append the PATH here with locations to our DDLs. Python won't pick the up.
)

set_property(
    TEST
        lue_framework_api_python_test
    APPEND
    PROPERTY
        FIXTURES_REQUIRED
            lue_py_test_fixture
)
