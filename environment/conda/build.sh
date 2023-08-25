#!/usr/bin/env bash
set -e
set -x

mkdir build

if [[ "$target_platform" == "osx-64" ]]; then
    export CXXFLAGS="${CXXFLAGS} -D_LIBCPP_DISABLE_AVAILABILITY"
fi

if [[ "$target_platform" == "osx-64" || "$target_platform" == "osx-arm64" ]]; then
    export CXXFLAGS="${CXXFLAGS} -DTARGET_OS_OSX"
fi

cat > host_profile << EOF
[settings]
arch=x86_64
build_type=Release
compiler=gcc
compiler.cppstd=17
compiler.libcxx=libstdc++11
compiler.version=$($CXX -dumpversion | sed 's/\..*//')
os=Linux
EOF

cat > build_profile << EOF
[settings]
arch=x86_64
build_type=Release
compiler=gcc
compiler.cppstd=17
compiler.libcxx=libstdc++11
compiler.version=$($CXX_FOR_BUILD -dumpversion | sed 's/\..*//')
os=Linux
EOF

echo "OSTYPE: $OSTYPE"

LUE_CONAN_PACKAGES="imgui span-lite" conan install . \
    --profile:build=build_profile \
    --profile:host=host_profile \
    --settings:host compiler.cppstd=17 \
    --settings:build compiler.cppstd=17 \
    --settings:build build_type=Release \
    --build=missing \
    --output-folder=build

CMAKE_PREFIX_PATH=build \
    cmake --preset conan-release \
        ${CMAKE_ARGS} \
        -D LUE_INSTALL_PYTHON_PACKAGE_DIR="${SP_DIR}/lue" \
        -D LUE_DATA_MODEL_WITH_PYTHON_API=TRUE \
        -D LUE_DATA_MODEL_WITH_UTILITIES=TRUE \
        -D LUE_BUILD_QA=TRUE \
        -D LUE_BUILD_VIEW=TRUE \
        -D LUE_QA_WITH_PYTHON_API=TRUE \
        -D LUE_FRAMEWORK_WITH_PYTHON_API=TRUE \
        -D HPX_IGNORE_COMPILER_COMPATIBILITY=TRUE \
        -D Python3_EXECUTABLE="${PYTHON}"

cmake --build build --target all
cmake --install build --component lue_runtime
