set(reference_target_name ${doc_preprocess_target_name}.reference)

add_custom_target(${reference_target_name})

add_subdirectory(operation)

add_dependencies(${reference_target_name}
    ${reference_target_name}.operation
)
