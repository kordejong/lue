set(example_nr 1)
set(example_target_name ${this_operation_target_name}.example-${example_nr})

link_example_arguments(
    TARGET
        ${example_target_name}.argument
    PATHNAMES
        ../../../argument/array/elevation-5x5-float32
)

add_operation_example(
    NAME
        ${operation_name}
    TARGET
        ${example_target_name}
    NR
        ${example_nr}
    LINK_LIBRARIES
        lue::framework_${operation_kind}_operation
    ARGUMENT_NAMES
        array/elevation-5x5-float32.tif
    RESULT_NAMES
        ${operation_name}-${example_nr}.tif
    DEPENDS
        ${example_target_name}.argument
)
