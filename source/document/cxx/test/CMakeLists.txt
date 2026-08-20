set(names
    example
)

add_unit_tests(
    SCOPE lue_document_cxx
    NAMES ${names}
    LIBRARIES
        lue_document_cxx_static
    EXPORT_MACRO_BASENAME
        DOCUMENT_CXX
    TARGETS
        test_names
)
