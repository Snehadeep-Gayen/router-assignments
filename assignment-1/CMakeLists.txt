cmake_minimum_required(VERSION 3.10)

# Project Name
project(NetworkSimulator)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)
include_directories(parser-lib/include)

# Source files
set(SOURCE_FILES
    src/main.cpp
    src/Simulator.cpp
    src/Controller.cpp
    src/NetworkDefs.cpp
    parser-lib/src/CommandLineParser.cpp
    parser-lib/src/Argument.cpp
    parser-lib/src/ArgumentInfo.cpp
)

# Add the executable
add_executable(NetworkSimulator ${SOURCE_FILES})

# Enable warnings
target_compile_options(NetworkSimulator PRIVATE -g -Wall -Wextra -Wpedantic)

# add tests and enable testing
add_subdirectory(tests)
enable_testing()
add_test(NAME RoutingTableTests COMMAND RoutingTableTests)

# Copy the json file to the output directory
file(COPY ${CMAKE_SOURCE_DIR}/src/params.json DESTINATION ${CMAKE_BINARY_DIR})