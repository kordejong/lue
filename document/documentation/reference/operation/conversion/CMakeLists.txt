set(operation_kind conversion)
set(${operation_kind}_operation_target_name ${operation_target_name}.${operation_kind})

add_custom_target(${${operation_kind}_operation_target_name})
