set(example_nr 1)
set(example_target_name ${operation_target_name}.example-${example_nr})

add_operation_example(
    NAME
        ${operation_name}
    TARGET
        ${example_target_name}
    NR
        ${example_nr}
    LINK_LIBRARIES
        lue::framework_${algorithm_group}_operation
        lue::framework_local_operation
    ARGUMENT_NAMES
    RESULT_NAMES
        ${operation_name}-${example_nr}.tif
)
