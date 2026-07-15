add_test(
    NAME lue_py_data_model_python_test
    COMMAND ${Python_EXECUTABLE}
        -m unittest discover
        --verbose
        --start-directory ${CMAKE_CURRENT_SOURCE_DIR}
        --pattern "*_test.py"
)

lue_configure_python_test(
    TARGET
        lue_py_data_model_python_test
)
