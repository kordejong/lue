set(documentation_target_name lue.documentation)
set(reference_target_name ${documentation_target_name}.reference)
set(operation_target_name ${reference_target_name}.operation)

add_custom_target(${operation_target_name})

add_subdirectory(argument)
add_dependencies(
    ${operation_target_name}
    ${operation_target_name}.argument
)

add_subdirectory(convolve)
add_dependencies(
    ${operation_target_name}
    ${operation_target_name}.convolve
)
