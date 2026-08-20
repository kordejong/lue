set(install_prefix
    "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/$<$<BOOL:${LUE_GENERATOR_IS_MULTI_CONFIG}>:$<CONFIG>>"
)

foreach(name lue_create_example_figure lue_import_example_data)
    set(module_name "${name}.py")

    file(GENERATE
        OUTPUT ${install_prefix}/${module_name}
        INPUT ${module_name}
    )

    lue_install_programs(TARGETS
        ${install_prefix}/${module_name}
    )
endforeach()
