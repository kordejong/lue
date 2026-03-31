set(operation_target_name ${reference_target_name}.operation)

add_custom_target(${operation_target_name})

add_subdirectory(argument)

set(operaration_kind_names
    focal
    local
    global
    miscellaneous
    routing
    zonal
)

foreach(operation_kind_name ${operaration_kind_names})
    add_subdirectory(${operation_kind_name})

    add_dependencies(${operation_target_name}
        ${operation_target_name}.${operation_kind_name}
    )
endforeach()
