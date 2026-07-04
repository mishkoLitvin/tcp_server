#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status
set -e

# Define directories
SOURCE_DIR="."
BUILD_DIR="build"
BUILD_TYPE="Release" # Options: Debug, Release, RelWithDebInfo, MinSizeRel

echo "=== Configuring CMake Project (${BUILD_TYPE}) ==="
cmake -S "${SOURCE_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=${BUILD_TYPE}

echo "=== Building Project ==="
cmake --build "${BUILD_DIR}" --config ${BUILD_TYPE}

cp "${BUILD_DIR}/tcp_server" "tcp_server"
echo "=== Build Complete ==="
