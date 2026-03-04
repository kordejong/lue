set(names
    example
)

add_unit_tests(
    SCOPE lue_document
    NAMES ${names}
    LIBRARIES
        lue_document_static
    EXPORT_MACRO_BASENAME
        DOCUMENT
    TARGETS
        test_names
)
