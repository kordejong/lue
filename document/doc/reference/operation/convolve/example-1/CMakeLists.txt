set(operation "convolve")
set(algorithm_group "focal")
set(example_nr "1")

set(argument1 "array/5x5-float32.tif")
set(argument2 "kernel/3x3-float32.tif")
set(result1_basename "convolve-${example_nr}")
set(result1 "${result1_basename}.tif")

set(argument_prefix "${CMAKE_CURRENT_BINARY_DIR}/../argument")
set(result_prefix "${CMAKE_CURRENT_BINARY_DIR}/result")

# Prepare argument data

# /${operation}/${algorithm_group}/
# /${operation}/${algorithm_group}/

# set(target_name "lue.documentation.reference.operation.${operation}.example-${example_nr}")
set(target_name "${operation}.example-${example_nr}")

add_executable(${target_name}
    ${operation}.cpp
)

target_link_libraries(${target_name}
    PRIVATE
        lue::document
        lue::framework_${algorithm_group}_operation
        HPX::wrap_main
)

add_custom_command(
    OUTPUT
        ${result_prefix}/${result1}
    DEPENDS
        ${argument_prefix}/${argument1}
        ${argument_prefix}/${argument2}
    COMMAND $<TARGET_FILE:${target_name}>
        ${argument_prefix}/${argument1}
        ${argument_prefix}/${argument2}
        ${result_prefix}/${result1}
    VERBATIM
)

add_custom_target(
    "lue.documentation.reference.operation.${operation}.example-${example_nr}"
    DEPENDS
        ${result_prefix}/${result1}
)

tif_to_figure(
    BASENAME
        ${result1_basename}
    TARGET ${ARG_TARGET}
        "lue.documentation.reference.operation.${operation}.example-${example_nr}.result"
    SOURCE_PREFIX
        ${result_prefix}
    DESTINATION_PREFIX
        ${result_prefix}
)

# TODO: Run Python example
