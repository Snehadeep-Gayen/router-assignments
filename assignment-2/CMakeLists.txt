cmake_minimum_required(VERSION 3.10)

# Set project name and C++ standard
project(SwitchNetwork VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
include_directories(include parser-lib/include)
include_directories(include include)

# Add executable for the main application
add_executable(SwitchNetwork src/main.cpp)

# Source files for the main executable
set(SOURCES
    src/main
    src/Delta.cpp
    src/Omega.cpp
    src/Switch.cpp
    src/SwitchNetwork.cpp
    src/Benes.cpp
)

target_sources(SwitchNetwork PRIVATE ${SOURCES})

# Link the parser-lib to the main executable
add_subdirectory(parser-lib)

target_link_libraries(SwitchNetwork PRIVATE ParserLibrary)

# Optionally enable testing
enable_testing()

# Install rules (optional)
install(TARGETS SwitchNetwork DESTINATION bin)

# Copy the params.json file to the build directory
add_custom_command(TARGET SwitchNetwork POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/src/params.json
    $<TARGET_FILE_DIR:SwitchNetwork>/params.json
)
