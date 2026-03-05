set(algorithm_group focal)
set(operation_name convolve)
set(convolve_target_name ${operation_target_name}.${operation_name})

add_subdirectory(argument)
add_subdirectory(example-1)

add_custom_target(${convolve_target_name})

add_dependencies(
    ${convolve_target_name}
    ${convolve_target_name}.example-1
)
