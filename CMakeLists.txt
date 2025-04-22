cmake_minimum_required(VERSION 3.27)
project(quantum_circuit_simulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET quantum_circuit_simulation)

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)
add_executable(${TARGET} ${SRC_FILES})

# # Enable testing
# include(CTest)
# enable_testing()

# # Fetch GoogleTest using Git
# include(FetchContent)
# FetchContent_Declare(googletest
#     GIT_REPOSITORY https://github.com/google/googletest.git
#     GIT_TAG release-1.14.0
#     GIT_SHALLOW ON
#     EXCLUDE_FROM_ALL
#     SYSTEM
# )
# FetchContent_MakeAvailable(googletest)

# # Add test executable
# file(GLOB_RECURSE TEST_SRC_FILES tests/*.cpp tests/*.h)
# add_executable(${TARGET}_tests ${TEST_SRC_FILES})

# target_link_libraries(${TARGET}_tests
#   gtest
#   gtest_main
#   ${TARGET}
# )

# include(GoogleTest)
# gtest_discover_tests(${TARGET}_tests)
