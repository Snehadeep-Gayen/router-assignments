cmake_minimum_required(VERSION 3.10)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# # Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add executable for the tests
add_executable(PacketSchedulingTests
    test_main.cpp
    ${PROJECT_SOURCE_DIR}/src/TrafficGenerator.cpp
)

# Optionally, enable testing (if you are running tests from command line)
enable_testing()
add_test(NAME PacketSchedulingTests COMMAND PacketSchedulingTests)