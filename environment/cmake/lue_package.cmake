set(CPACK_VERBATIM_VARIABLES TRUE)

set(CPACK_PACKAGE_NAME ${LUE_LOWER_PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "LUE R&D team")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_DESCRIPTION_FILE ${${PROJECT_NAME}_SOURCE_DIR}/document/doc/short_description.rst)

set(CPACK_SOURCE_PACKAGE_FILE_NAME ${LUE_LOWER_PROJECT_NAME}-${LUE_VERSION})
set(CPACK_SOURCE_IGNORE_FILES
    /\\.cache/
    /\\.git/
    /\\.github/
    /\\.gitignore
    /\\.gitmodules
    /\\.clang-format
    /compile_commands.json
    /conanfile.py
    /\\.clang-tidy
    /\\.mypy_cache/
    /.*Presets.*\\.json
    out
    /.pre-commit-config.yaml
    /build_profile
    /host_profile
    /pyproject.toml
    /tags
    .*\\.pyc
    .*\\.in
    .*\\.swp
    /env
    __pycache__/
)

if(WIN32)
    set(CPACK_GENERATOR ZIP)
    set(CPACK_SOURCE_GENERATOR ZIP)
else()
    set(CPACK_GENERATOR TGZ)
    set(CPACK_SOURCE_GENERATOR TGZ)
endif()

include(CPack)
