cmake_minimum_required(VERSION 3.10)

# Set project name and C++ standard
project(spt VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=1")
endif()


# Set debug mode
set(CMAKE_BUILD_TYPE Debug)

# Include directories
include_directories(include parser-lib/include)
include_directories(include include)

# Add executable for the main application
add_executable(spt)

# Source files for the main executable
set(SOURCES
    src/main.cpp
    src/simulator.cpp
)

target_sources(spt PRIVATE ${SOURCES})

# Link the parser-lib to the main executable
add_subdirectory(parser-lib)

target_link_libraries(spt PRIVATE ParserLibrary)

# Optionally enable testing
enable_testing()

# Install rules (optional)
install(TARGETS spt DESTINATION bin)

# Copy the params.json file to the build directory
add_custom_command(TARGET spt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/src/params.json
    $<TARGET_FILE_DIR:spt>/params.json
)

# Add the test directory
add_subdirectory(test)