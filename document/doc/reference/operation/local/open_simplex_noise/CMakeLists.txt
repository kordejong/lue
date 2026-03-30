set(operation_name open_simplex_noise)
set(nr_examples 1)

set(this_operation_target_name ${${operation_kind}_operation_target_name}.${operation_name})

add_custom_target(${this_operation_target_name})

foreach(example_nr RANGE 1 ${nr_examples})
    add_subdirectory(example-${example_nr})

    add_dependencies(${this_operation_target_name}
        ${this_operation_target_name}.example-${example_nr}
    )
endforeach()
