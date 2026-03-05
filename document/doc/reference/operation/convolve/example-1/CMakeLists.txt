set(example_nr 1)

set(argument1 array/5x5-float32.tif)
set(argument2 kernel/3x3-float32.tif)
set(result1_basename convolve-${example_nr})
set(result1 ${result1_basename}.tif)

set(argument_prefix "${CMAKE_CURRENT_BINARY_DIR}/../argument")
set(result_prefix "${CMAKE_CURRENT_BINARY_DIR}/result")

set(example_target_name ${convolve_target_name}.example-${example_nr})

add_custom_target(${example_target_name})

# TODO: Refactor into a function ----------------------------
set(language cxx)

set(executable_target_name ${operation_name}.example-${example_nr})

file(GENERATE
    OUTPUT ${operation_name}.cpp
    INPUT ${operation_name}.cpp
)

add_executable(${executable_target_name}
    ${operation_name}.cpp
)

target_link_libraries(${executable_target_name}
    PRIVATE
        lue::document
        lue::framework_${algorithm_group}_operation
        HPX::wrap_main
)

add_custom_command(
    OUTPUT
        ${result_prefix}/${language}/${result1}
    DEPENDS
        ${argument_prefix}/${argument1}
        ${argument_prefix}/${argument2}
    COMMAND $<TARGET_FILE:${executable_target_name}>
        ${argument_prefix}/${argument1}
        ${argument_prefix}/${argument2}
        ${result_prefix}/${language}/${result1}
    VERBATIM
)

add_custom_target(
    ${example_target_name}.${language}.result
    DEPENDS
        ${result_prefix}/${language}/${result1}
)

tif_to_figure(
    BASENAME
        ${result1_basename}
    TARGET ${ARG_TARGET}
        ${example_target_name}.${language}.result.figure
    SOURCE_PREFIX
        ${result_prefix}/${language}
    DESTINATION_PREFIX
        ${result_prefix}/${language}
)

add_dependencies(${example_target_name}
    ${convolve_target_name}.argument
    ${example_target_name}.${language}.result
    ${example_target_name}.${language}.result.figure
)

# TODO: Refactor into a function ----------------------------
set(language python)

file(GENERATE
    OUTPUT ${operation_name}.py
    INPUT ${operation_name}.py
)

add_custom_command(
    OUTPUT
        ${result_prefix}/${language}/${result1}
    DEPENDS
        ${argument_prefix}/${argument1}
        ${argument_prefix}/${argument2}
        ${CMAKE_CURRENT_SOURCE_DIR}/convolve.py
    COMMAND
        ${CMAKE_COMMAND} -E env PYTHONPATH=$<TARGET_FILE_DIR:lue::py>/.. -- ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/convolve.py"
        ${argument_prefix}/${argument1}
        ${argument_prefix}/${argument2}
        ${result_prefix}/${language}/${result1}
    VERBATIM
)

add_custom_target(
    ${example_target_name}.${language}.result
    DEPENDS
        ${result_prefix}/${language}/${result1}
)

tif_to_figure(
    BASENAME
        ${result1_basename}
    TARGET ${ARG_TARGET}
        ${example_target_name}.${language}.result.figure
    SOURCE_PREFIX
        ${result_prefix}/${language}
    DESTINATION_PREFIX
        ${result_prefix}/${language}
)

add_dependencies(
    ${example_target_name}
        ${example_target_name}.${language}.result
        ${example_target_name}.${language}.result.figure
)
