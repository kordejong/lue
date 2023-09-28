echo on

mkdir build
if errorlevel 1 exit 1

%PYTHON% environment/script/write_conan_profile.py %CXX% host_profile
if errorlevel 1 exit 1

%PYTHON% environment/script/write_conan_profile.py %CXX% build_profile
if errorlevel 1 exit 1

set LUE_CONAN_PACKAGES=imgui

conan install . ^
    --profile:host=host_profile ^
    --profile:build=build_profile ^
    --build=missing ^
    --output-folder=build
if errorlevel 1 exit 1

:: Unset CMAKE_GENERATOR_PLATFORM. The platform is already specified in Conan's preset. Otherwise:
:: Generator
::     Ninja
:: does not support platform specification, but platform
::     x64
:: was specified.
set CMAKE_GENERATOR_PLATFORM=
set CMAKE_PREFIX_PATH=build

cmake --preset conan-release ^
    -D CMAKE_INSTALL_PREFIX:PATH="%LIBRARY_PREFIX%" ^
    -D CMAKE_INSTALL_LIBDIR=lib ^
    -D LUE_INSTALL_PYTHON_PACKAGE_DIR="%SP_DIR%/lue" ^
    -D LUE_DATA_MODEL_WITH_PYTHON_API=TRUE ^
    -D LUE_DATA_MODEL_WITH_UTILITIES=TRUE ^
    -D LUE_BUILD_VIEW=TRUE ^
    -D LUE_BUILD_QA=TRUE ^
    -D LUE_QA_WITH_PYTHON_API=TRUE ^
    -D LUE_FRAMEWORK_WITH_PYTHON_API=TRUE ^
    -D HPX_IGNORE_COMPILER_COMPATIBILITY=TRUE ^
    -D Python3_EXECUTABLE="%PYTHON%"
if errorlevel 1 exit 1

cmake --build build --target all
if errorlevel 1 exit 1

cmake --install build --component lue_runtime
if errorlevel 1 exit 1
