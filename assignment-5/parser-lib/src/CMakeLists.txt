# Create a library from the source files
add_library(ParserLibrary
    Argument.cpp
    ArgumentInfo.cpp
    CommandLineParser.cpp
)

# Optionally, specify include directories for this library
target_include_directories(ParserLibrary PUBLIC ${PROJECT_SOURCE_DIR}/include)
