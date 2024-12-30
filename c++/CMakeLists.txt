cmake_minimum_required(VERSION 3.27)

if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
else()
    message(FATAL_ERROR "VCPKG_ROOT is not set. Please set it to the root of your vcpkg installation.")
endif()

project(QuantumCircuitSimulation)

enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-Wall -Wextra -pedantic -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG)
endif()

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(BENCHMARK_DIR ${CMAKE_SOURCE_DIR}/benchmarks)

set(TARGET main_executable)
set(TEST_TARGET tests)
set(BENCHMARK_TARGET benchmarks)

file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp)

add_executable(${TARGET} ${SRC_FILES})


find_package(GTest CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)

include_directories(${SRC_DIR})


file(GLOB_RECURSE TEST_FILES ${TEST_DIR}/*.cpp)
if(TEST_FILES)
    add_executable(${TEST_TARGET} ${TEST_FILES})
    target_link_libraries(${TEST_TARGET} GTest::gtest_main GTest::gmock GTest::gmock_main)
    include(GoogleTest)
    gtest_discover_tests(${TEST_TARGET})
endif()

file(GLOB_RECURSE BENCHMARK_FILES ${BENCHMARK_DIR}/*.cpp)
if(BENCHMARK_FILES)
    add_executable(${BENCHMARK_TARGET} ${BENCHMARK_FILES})
    target_link_libraries(${BENCHMARK_TARGET} benchmark::benchmark)
endif()
