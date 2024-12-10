cmake_minimum_required(VERSION 3.30)
project(QuantumCircuitSimulation)

# Enable testing and benchmarks
enable_testing()

# Set the CMake toolchain file for vcpkg
# TODO: dont hardcode the path for vcpkg.cmake 
set(CMAKE_TOOLCHAIN_FILE /Users/ivan/vcpkg/scripts/buildsystems/vcpkg.cmake)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(BENCHMARK_DIR ${CMAKE_SOURCE_DIR}/benchmarks)

# Define the target name for the main executable
set(TARGET main_executable)

# Add source files
file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp)

# Main application executable
add_executable(${TARGET} ${SRC_FILES})

# Use vcpkg to find GTest and Google Benchmark
find_package(GTest CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)

# Include directories
include_directories(${SRC_DIR})

# Link libraries for main executable
target_link_libraries(${TARGET} PRIVATE GTest::gtest GTest::gtest_main)

# Add tests
file(GLOB_RECURSE TEST_FILES ${TEST_DIR}/*.cpp)
if(TEST_FILES)
    add_executable(tests ${TEST_FILES})
    # target_link_libraries(tests GTest::gtest_main)
    target_link_libraries(tests GTest::gtest_main GTest::gmock GTest::gmock_main)
    include(GoogleTest)
    gtest_discover_tests(tests)
endif()

# Add benchmarks
file(GLOB_RECURSE BENCHMARK_FILES ${BENCHMARK_DIR}/*.cpp)
if(BENCHMARK_FILES)
    add_executable(benchmarks ${BENCHMARK_FILES})
    target_link_libraries(benchmarks benchmark::benchmark)
endif()

# Install targets (optional)
install(TARGETS ${TARGET} tests benchmarks RUNTIME DESTINATION bin)

