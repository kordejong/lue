set(operation_target_name ${reference_target_name}.operation)

add_custom_target(${operation_target_name})

add_subdirectory(argument)

set(operation_names
    convolve
    d8_flow_direction
    some_operation
    open_simplex_noise
)

foreach(operation_name ${operation_names})
    add_subdirectory(${operation_name})

    add_dependencies(${operation_target_name}
        ${operation_target_name}.${operation_name}
    )
endforeach()
