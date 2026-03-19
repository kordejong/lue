set(example_nr 1)
set(example_target_name ${operation_target_name}.example-${example_nr})

add_custom_command(
    OUTPUT
        argument/array/5x5-float32.tif
    DEPENDS
        ${operation_target_name}.argument
    COMMAND
        ${CMAKE_COMMAND} -E make_directory argument/array
    COMMAND
        ${CMAKE_COMMAND} -E create_symlink
            ../../../argument/array/5x5-float32.tif
            argument/array/5x5-float32.tif
    VERBATIM
)

add_operation_example(
    NAME
        ${operation_name}
    TARGET
        ${example_target_name}
    NR
        ${example_nr}
    LINK_LIBRARIES
        lue::framework_${algorithm_group}_operation
    ARGUMENT_NAMES
        array/5x5-float32.tif
    RESULT_NAMES
        ${operation_name}-${example_nr}.tif
)
