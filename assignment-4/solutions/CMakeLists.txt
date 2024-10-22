cmake_minimum_required(VERSION 3.10)

# Set project name and C++ standard
project(wfq VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set debug mode
set(CMAKE_BUILD_TYPE Debug)

# Enable optimizations for Release builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")


# Include directories
include_directories(include parser-lib/include)
include_directories(include include)

# Add executable for the main application
add_executable(wfq)

# Source files for the main executable
set(SOURCES
    src/Main.cpp
    src/PacketGen.cpp
    src/Scheduler.cpp
)

target_sources(wfq PRIVATE ${SOURCES})

# Link the parser-lib to the main executable
add_subdirectory(parser-lib)

target_link_libraries(wfq PRIVATE ParserLibrary)

# Optionally enable testing
enable_testing()

# Install rules (optional)
install(TARGETS wfq DESTINATION bin)

# Copy the params.json file to the build directory
add_custom_command(TARGET wfq POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/src/params.json
    $<TARGET_FILE_DIR:wfq>/params.json
)


# Add the test directory
add_subdirectory(test)