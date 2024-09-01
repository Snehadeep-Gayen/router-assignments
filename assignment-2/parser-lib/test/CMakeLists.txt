cmake_minimum_required(VERSION 3.10)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# # Add the include directory
# include_directories(${PROJECT_SOURCE_DIR}/include)

# Add executable for the tests
add_executable(ParserLibraryTests
    test_main.cpp
    # Add your test files here
    cli.json
)

# Link the library to the test executable
target_link_libraries(ParserLibraryTests
    ParserLibrary
    pthread
)

# Optionally, enable testing (if you are running tests from command line)
enable_testing()
add_test(NAME ParserLibraryTests COMMAND ParserLibraryTests)

# Copy the cli.json file to the build directory
file(COPY ${PROJECT_SOURCE_DIR}/test/cli.json DESTINATION ${CMAKE_BINARY_DIR}/test)
