add_test(
    NAME lue_py_qa_python_test
    COMMAND ${Python_EXECUTABLE}
        -m unittest discover
        --verbose
        --start-directory ${CMAKE_CURRENT_SOURCE_DIR}
        --pattern "*_test.py"
)

set_property(
    TEST
        lue_py_qa_python_test
    APPEND
    PROPERTY
        ENVIRONMENT_MODIFICATION
            # Preload the custom allocator shared library, if necessary
            LD_PRELOAD=path_list_append:$<$<AND:$<PLATFORM_ID:Linux>,$<BOOL:${Jemalloc_LIBRARY}>>:${Jemalloc_LIBRARY}>
            LD_PRELOAD=path_list_append:$<$<AND:$<PLATFORM_ID:Linux>,$<BOOL:${Tcmalloc_LIBRARY}>>:${Tcmalloc_LIBRARY}>

            # Add the path to our Python modules to PYTHONPATH.
            PYTHONPATH=path_list_prepend:$<TARGET_FILE_DIR:lue::py>/..
            # No need to append the PATH here with locations to our DDLs. Python won't pick them up.
)

set_property(
    TEST
        lue_py_qa_python_test
    APPEND
    PROPERTY
        FIXTURES_REQUIRED
            lue_py_test_fixture
)
