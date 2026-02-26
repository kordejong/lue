set(nr_rows "5")
set(nr_cols "5")
set(element_type "float32")
set(name "${nr_rows}x${nr_cols}-${element_type}")

set(phenomenon "phenomenon")
set(property_set "area")
set(property_name "${name}")

configure_file(
    "meta.json.in"
    "${name}-meta.json"
)

# TODO: Import no-data value from json to lue
add_custom_command(
    OUTPUT "${name}.lue"
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/${name}.json"
    COMMAND "$<TARGET_FILE:lue_translate>" import
        "${CMAKE_CURRENT_BINARY_DIR}/${name}.lue"
        "${CMAKE_CURRENT_SOURCE_DIR}/${name}.json"
    VERBATIM
)

# TODO: Export no-data value from lue to tif
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

add_custom_target("lue.documentation.reference.operation.array.${name}"
    DEPENDS "${name}.tif"
)
