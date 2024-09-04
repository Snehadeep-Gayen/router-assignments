cmake_minimum_required(VERSION 3.10)

project(ParserLibrary)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add the src directory
add_subdirectory(src)

# Add the test directory
add_subdirectory(test)
