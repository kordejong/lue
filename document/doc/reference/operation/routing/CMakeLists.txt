set(operation_kind routing)
set(${operation_kind}_operation_target_name ${operation_target_name}.${operation_kind})

add_custom_target(${${operation_kind}_operation_target_name})

add_subdirectory(argument)

set(operation_names
    d8_flow_direction
)

foreach(operation_name ${operation_names})
    add_subdirectory(${operation_name})

    add_dependencies(${${operation_kind}_operation_target_name}
        ${${operation_kind}_operation_target_name}.${operation_name}
    )
endforeach()
