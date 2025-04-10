# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.12)

# add the header files
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/parser-lib/include)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set debugging symbols
set(CMAKE_BUILD_TYPE Debug)

# Set the project name
project(Assignment1Tests)

# Add the executable targets for each test file
add_executable(TablesAndConnectionsTests TablesAndConnections.cpp ${CMAKE_SOURCE_DIR}/src/NetworkDefs.cpp)
# add_executable(test2 test2.cpp)
# add_executable(test3 test3.cpp)

# Set the output directory for the executables
set_target_properties(TablesAndConnectionsTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)