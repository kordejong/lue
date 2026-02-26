set(operation "convolve")
set(algorithm_group "focal")
set(example_nr "1")

set(argument1, "array/5x5-float32.tif")
set(argument2, "kernel/3x3-float32.tif")
set(result1, "convolve-${example_nr}/tif")

set(argument_prefix, "${CMAKE_CURRENT_BINARY_DIR}/argument/array")

# Prepare argument data


# /${operation}/${algorithm_group}/
# /${operation}/${algorithm_group}/


set(target_name "lue.documentation.reference.operation.${operation}.example-${example_nr}")

add_executable(${target_name}
    ${operation}.cpp
)

target_link_libraries(${target_name}
    PRIVATE
        lue::framework_${algorithm_group}_operation
        cxxopts::cxxopts
        HPX::wrap_main
)

add_custom_command(
    OUTPUT ${result1} ${result2}
    COMMAND ${TARGET_FILE:target_name}
        ${argument_prefix}/${argument1}
        ${argument_prefix}/${argument2}
        ${result_prefix}/${result1}
)

# TODO: Run Python example
