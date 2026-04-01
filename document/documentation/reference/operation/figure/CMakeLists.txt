# TODO: All this requires:
# - LaTeX
# - dvisvgm
# Bubble these requirements up to the documentation target. Similar as is done with the logo target (or should
# be).

set(operation_figure_target_name ${operation_target_name}.figure)

add_custom_target(${operation_figure_target_name})

foreach(operation_kind_name
        focal
        local
        # TODO: global
        routing
        zonal
    )
    set(target_name "${operation_figure_target_name}.${operation_kind_name}")

    add_latex_document(${operation_kind_name}_operation.tex
        TARGET_NAME ${target_name}
        INPUTS
            operation.tex
    )

    add_custom_command(
        TARGET ${target_name}
        POST_BUILD
        COMMENT "${operation_kind_name}_operation.pdf → ${operation_kind_name}_operation.svg"
        BYPRODUCTS "${operation_kind_name}_operation.svg"
        COMMAND ${dvisvgm_EXECUTABLE} --verbosity=0 --pdf ${operation_kind_name}_operation.pdf ${operation_kind_name}_operation.svg
        VERBATIM
    )

    add_dependencies(${operation_figure_target_name}
        ${operation_figure_target_name}.${operation_kind_name}
    )
endforeach()
