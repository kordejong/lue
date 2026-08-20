set(argument_target_name ${operation_target_name}.argument)

add_subdirectory(array)
# add_subdirectory(kernel)

add_custom_target(${argument_target_name})

add_dependencies(${argument_target_name}
    ${argument_target_name}.array.figure
#     ${argument_target_name}.kernel.figure
)
