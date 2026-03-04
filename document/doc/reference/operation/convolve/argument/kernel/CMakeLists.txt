set(nr_rows "3")
set(nr_cols "3")
set(element_type "uint8")
set(name "${nr_rows}x${nr_cols}-${element_type}")

add_custom_command(
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/${name}.json"
        "${CMAKE_CURRENT_BINARY_DIR}/${name}-meta.json"
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/${name}.txt"
    COMMAND
        ${CMAKE_COMMAND} -E env PYTHONPATH=$<TARGET_FILE_DIR:lue::py>/.. -- ${Python_EXECUTABLE} "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/$<$<BOOL:${LUE_GENERATOR_IS_MULTI_CONFIG}>:$<CONFIG>>/lue_import_example_data.py"
        "${CMAKE_CURRENT_SOURCE_DIR}/${name}.txt"
        "${CMAKE_CURRENT_BINARY_DIR}/${name}.json"
        "${CMAKE_CURRENT_BINARY_DIR}/${name}-meta.json"
    VERBATIM
)

add_custom_command(
    OUTPUT "${name}.lue"
    DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/${name}.json"
    COMMAND "$<TARGET_FILE:lue_translate>" import
        "${CMAKE_CURRENT_BINARY_DIR}/${name}.lue"
        "${CMAKE_CURRENT_BINARY_DIR}/${name}.json"
    VERBATIM
)

add_custom_command(
    OUTPUT "${name}.tif"
    DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/${name}.lue"
        "${CMAKE_CURRENT_BINARY_DIR}/${name}-meta.json"
    COMMAND "$<TARGET_FILE:lue_translate>" export
        --meta "${CMAKE_CURRENT_BINARY_DIR}/${name}-meta.json"
        "${name}.lue" "${CMAKE_CURRENT_BINARY_DIR}/${name}.tif"
    VERBATIM
)

add_custom_command(
    OUTPUT
        "${name}.pdf"
        "${name}.svg"
    DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/${name}.tif"
    COMMAND
        ${CMAKE_COMMAND} -E env PYTHONPATH=$<TARGET_FILE_DIR:lue::py>/.. -- ${Python_EXECUTABLE} "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/$<$<BOOL:${LUE_GENERATOR_IS_MULTI_CONFIG}>:$<CONFIG>>/lue_create_example_figure.py"
        "${CMAKE_CURRENT_BINARY_DIR}/${name}.tif"
        "${CMAKE_CURRENT_BINARY_DIR}/${name}"
        pdf svg
    VERBATIM
)

add_custom_target("lue.documentation.reference.operation.kernel.${name}"
    DEPENDS "${name}.svg"
)
