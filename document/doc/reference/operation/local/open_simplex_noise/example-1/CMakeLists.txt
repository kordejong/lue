set(example_nr 1)
set(example_target_name ${this_operation_target_name}.example-${example_nr})

add_operation_example(
    NAME
        ${operation_name}
    TARGET
        ${example_target_name}
    NR
        ${example_nr}
    LINK_LIBRARIES
        lue::framework_${operation_kind}_operation
        # TODO: Move open_simplex_noise into local operation and remove this lib
        lue::framework_miscellaneous_operation
    RESULT_NAMES
        ${operation_name}-${example_nr}.tif
)
