cmake_minimum_required(VERSION 3.10)

# Set project name and C++ standard
project(PacketQueueing VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set debug mode
set(CMAKE_BUILD_TYPE Debug)

# Include directories
include_directories(include parser-lib/include)
include_directories(include include)

# Add executable for the main application
add_executable(PacketQueueing src/main.cpp)

# Source files for the main executable
set(SOURCES
    src/main
    src/Simulator.cpp
    src/TrafficGenerator.cpp
)

target_sources(PacketQueueing PRIVATE ${SOURCES})

# Link the parser-lib to the main executable
add_subdirectory(parser-lib)

target_link_libraries(PacketQueueing PRIVATE ParserLibrary)

# Optionally enable testing
enable_testing()

# Install rules (optional)
install(TARGETS PacketQueueing DESTINATION bin)

# Copy the params.json file to the build directory
add_custom_command(TARGET PacketQueueing POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/src/params.json
    $<TARGET_FILE_DIR:PacketQueueing>/params.json
)


# Add the test directory
add_subdirectory(test)