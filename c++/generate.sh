
# Check for VCPKG_ROOT environment variable
if [[ -z "$VCPKG_ROOT" ]]; then
    echo "Error: VCPKG_ROOT is not set. Please set it to the path of your vcpkg installation."
    exit 1
fi

# Set the vcpkg toolchain file path
VCPKG_PATH="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

# Check if vcpkg toolchain file exists
if [[ ! -f "$VCPKG_PATH" ]]; then
    echo "Error: vcpkg toolchain file not found at $VCPKG_PATH"
    exit 1
fi

# Ensure the user provides a valid build type
BUILD_TYPE=$1
if [[ "$BUILD_TYPE" == "debug" || "$BUILD_TYPE" == "release" ]]; then
    if [[ "$BUILD_TYPE" == "debug" ]]; then
        BUILD_TYPE_CAPITALIZED="Debug"
    elif [[ "$BUILD_TYPE" == "release" ]]; then
        BUILD_TYPE_CAPITALIZED="Release"
    fi

    mkdir -p build/$BUILD_TYPE
    cd build/$BUILD_TYPE
    cmake   -DCMAKE_CXX_COMPILER=g++ \
            -DCMAKE_BUILD_TYPE=$BUILD_TYPE_CAPITALIZED \
            -DCMAKE_TOOLCHAIN_FILE="$VCPKG_PATH" \
            ../..
    
else
    echo "Usage: ./generate.sh [debug|release]"
    exit 1
fi
