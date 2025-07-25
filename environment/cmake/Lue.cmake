set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include(CompilerCache)
useCompilerCache()

include(FetchContent)
include(GenerateExportHeader)

set(LUE_MIN_CXX_STANDARD 20)

set(CMAKE_CXX_SCAN_FOR_MODULES FALSE)

find_package(LATEX)
if(LATEX_FOUND)
    include(UseLATEX)
    find_package(ImageMagick)
endif()

# Handle configuration and 3rd party libraries and tools, and ...
include(LueConfiguration)

# ... only now set settings relevant for (only) our own targets
# set(CMAKE_CXX_STANDARD ${LUE_MIN_CXX_STANDARD})
# set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)
endif()

include(LueMacro)

if(LUE_BUILD_DATA_MODEL)
    include(TestBigEndian)
    test_big_endian(IS_BIG_ENDIAN)
    if(IS_BIG_ENDIAN)
        message(FATAL
            "${PROJECT_NAME} currently assumes a little endian architecture")
    endif()
endif()

if(LUE_BUILD_QUALITY_ASSURANCE AND LUE_QUALITY_ASSURANCE_WITH_TESTS)
    enable_testing()
endif()

if(APPLE)
    set(LUE_ORIGIN @loader_path)
else()
    set(LUE_ORIGIN $ORIGIN)
endif()

include(GNUInstallDirs)

# If this project in included by another project also including GNUInstallDirs, then
# CMAKE_INSTALL_DOCDIR points to this other project's doc dir. Override this default
# behaviour by using a directory containing our PROJECT_NAME.
set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DATAROOTDIR}/doc/${LUE_LOWER_PROJECT_NAME})

file(RELATIVE_PATH LUE_BIN_TO_LIB_DIR
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
)

# In this project we need to be able to install the Python package in a certain directory. For
# that, we create a variable similar to the ones set by GNUInstallDirs. It can be overridden by
# the user.
if(Python_FOUND)
    set(LUE_BUILD_PYTHON_PACKAGE_DIR
        "${CMAKE_INSTALL_LIBDIR}/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/${LUE_LOWER_PROJECT_NAME}")

    set(LUE_INSTALL_PYTHON_PACKAGE_DIR "${LUE_BUILD_PYTHON_PACKAGE_DIR}" CACHE PATH
        "Relative (to install prefix) pathname of directory to install Python package in")

    if(NOT IS_ABSOLUTE ${CMAKE_INSTALL_PREFIX})
        message(SEND_ERROR "Pass in an absolute install prefix, or update this logic")
    endif()

    if(IS_ABSOLUTE ${LUE_INSTALL_PYTHON_PACKAGE_DIR})
        # Conda build procedure passes in an absolute path.
        file(RELATIVE_PATH LUE_PYTHON_PACKAGE_TO_LIB_DIR
            ${LUE_INSTALL_PYTHON_PACKAGE_DIR}
            ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
        )
    else()
        file(RELATIVE_PATH LUE_PYTHON_PACKAGE_TO_LIB_DIR
            ${CMAKE_INSTALL_PREFIX}/${LUE_INSTALL_PYTHON_PACKAGE_DIR}
            ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
        )
    endif()
endif()


message(STATUS "LUE_VERSION                   : ${LUE_VERSION}")
message(STATUS "LUE_GIT_SHA1                  : ${LUE_GIT_SHA1}")
message(STATUS "LUE_GIT_SHORT_SHA1            : ${LUE_GIT_SHORT_SHA1}")
message(STATUS "Build data model              : ${LUE_BUILD_DATA_MODEL}")
message(STATUS "+ python api                  : ${LUE_DATA_MODEL_WITH_PYTHON_API}")
message(STATUS "+ utilities                   : ${LUE_DATA_MODEL_WITH_UTILITIES}")
message(STATUS "Build framework               : ${LUE_BUILD_FRAMEWORK}")
message(STATUS "+ python api                  : ${LUE_FRAMEWORK_WITH_PYTHON_API}")
message(STATUS "+ parallel i/o                : ${LUE_FRAMEWORK_WITH_PARALLEL_IO}")
message(STATUS "Build view                    : ${LUE_BUILD_VIEW}")
message(STATUS "+ value inspection            : ${LUE_BUILD_FRAMEWORK}")
message(STATUS "Build documentation           : ${LUE_BUILD_DOCUMENTATION}")
message(STATUS "Build quality assurance       : ${LUE_BUILD_QUALITY_ASSURANCE}")
message(STATUS "+ python api                  : ${LUE_QUALITY_ASSURANCE_WITH_PYTHON_API}")
message(STATUS "+ tests                       : ${LUE_QUALITY_ASSURANCE_WITH_TESTS}")
message(STATUS "")
message(STATUS "CMAKE_GENERATOR               : ${CMAKE_GENERATOR}")
message(STATUS "GENERATOR_IS_MULTI_CONFIG     : ${GENERATOR_IS_MULTI_CONFIG}")
message(STATUS "CMAKE_SOURCE_DIR              : ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR              : ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_INSTALL_PREFIX          : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_INSTALL_BINDIR          : ${CMAKE_INSTALL_BINDIR}")
message(STATUS "CMAKE_INSTALL_LIBDIR          : ${CMAKE_INSTALL_LIBDIR}")
message(STATUS "CMAKE_INSTALL_LIBEXECDIR      : ${CMAKE_INSTALL_LIBEXECDIR}")
message(STATUS "CMAKE_INSTALL_INCLUDEDIR      : ${CMAKE_INSTALL_INCLUDEDIR}")
message(STATUS "CMAKE_INSTALL_DOCDIR          : ${CMAKE_INSTALL_DOCDIR}")
if(Python_FOUND)
    message(STATUS "LUE_INSTALL_PYTHON_PACKAGE_DIR: ${LUE_INSTALL_PYTHON_PACKAGE_DIR}")
    message(STATUS "LUE_PYTHON_PACKAGE_TO_LIB_DIR : ${LUE_PYTHON_PACKAGE_TO_LIB_DIR}")
endif()
message(STATUS "LUE_BIN_TO_LIB_DIR            : ${LUE_BIN_TO_LIB_DIR}")
# message(STATUS "")
# message(STATUS "CMAKE_INSTALL_RPATH       : ${CMAKE_INSTALL_RPATH}")

if(LUE_BUILD_FRAMEWORK)
    message(STATUS "")
    message(STATUS "Templates will be instantiated for these policies, types and ranks:")
    message(STATUS "    LUE_FRAMEWORK_SIGNED_INTEGRAL_ELEMENTS     : ${LUE_FRAMEWORK_SIGNED_INTEGRAL_ELEMENTS}")
    message(STATUS "    LUE_FRAMEWORK_UNSIGNED_INTEGRAL_ELEMENTS   : ${LUE_FRAMEWORK_UNSIGNED_INTEGRAL_ELEMENTS}")
    message(STATUS "    LUE_FRAMEWORK_FLOATING_POINT_ELEMENTS      : ${LUE_FRAMEWORK_FLOATING_POINT_ELEMENTS}")
    message(STATUS "    LUE_FRAMEWORK_INDEX_ELEMENT                : ${LUE_FRAMEWORK_INDEX_ELEMENT}")
    message(STATUS "    LUE_FRAMEWORK_COUNT_ELEMENT                : ${LUE_FRAMEWORK_COUNT_ELEMENT}")
    message(STATUS "    LUE_FRAMEWORK_BOOLEAN_ELEMENT              : ${LUE_FRAMEWORK_BOOLEAN_ELEMENT}")
    message(STATUS "    LUE_FRAMEWORK_ID_ELEMENT                   : ${LUE_FRAMEWORK_ID_ELEMENT}")
    message(STATUS "    LUE_FRAMEWORK_FLOW_DIRECTION_ELEMENT       : ${LUE_FRAMEWORK_FLOW_DIRECTION_ELEMENT}")
    message(STATUS "    LUE_FRAMEWORK_MATERIAL_ELEMENTS            : ${LUE_FRAMEWORK_MATERIAL_ELEMENTS}")
    message(STATUS "    LUE_FRAMEWORK_ZONE_ELEMENTS                : ${LUE_FRAMEWORK_ZONE_ELEMENTS}")
    message(STATUS "    LUE_FRAMEWORK_RANKS                        : ${LUE_FRAMEWORK_RANKS}")
    message(STATUS "")
    message(STATUS "    LUE_FRAMEWORK_LOCALITY_ID_ELEMENT_AVAILABLE: ${LUE_FRAMEWORK_LOCALITY_ID_ELEMENT_AVAILABLE}")
    message(STATUS "    LUE_FRAMEWORK_CLOCK_TICK_ELEMENT_AVAILABLE : ${LUE_FRAMEWORK_CLOCK_TICK_ELEMENT_AVAILABLE}")

    if(LUE_VALIDATE_IDXS)
        message(WARNING "LUE_VALIDATE_IDXS is set. Turn this off for production use!")
    endif()
endif()
