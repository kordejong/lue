set(example_nr 1)
set(example_target_name ${this_operation_target_name}.example-${example_nr})

link_example_arguments(
    TARGET
        ${example_target_name}.argument
    PATHNAMES
        ../argument/array/5x5-float32
        ../argument/kernel/3x3-float32
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
        kernel/3x3-float32.tif
    RESULT_NAMES
        ${operation_name}-${example_nr}.tif
    DEPENDS
        ${example_target_name}.argument
)
