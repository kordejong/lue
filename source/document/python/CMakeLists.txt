add_subdirectory(script)

set(install_prefix
    "${CMAKE_BINARY_DIR}/${LUE_BUILD_PYTHON_PACKAGE_DIR}/$<$<BOOL:${LUE_GENERATOR_IS_MULTI_CONFIG}>:$<CONFIG>>/document"
)

foreach(module_name
    "__init__.py"
    "cli/__init__.py"
    "cli/create_example_figure.py"
    "cli/import_example_data.py"
    "cli/main.py"
    "data/__init__.py"
    "figure/__init__.py"
    "figure/map/__init__.py"
    "io/__init__.py"
)
    file(GENERATE
        OUTPUT "${install_prefix}/${module_name}"
        INPUT "package/document/${module_name}"
    )
endforeach()

install(
    DIRECTORY "${install_prefix}"
    DESTINATION "${LUE_INSTALL_PYTHON_PACKAGE_DIR}"
    COMPONENT lue_runtime
)
