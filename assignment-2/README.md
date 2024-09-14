# Switching Backplane Configurations - Omega, Delta \& Benes

## Working

There are no bugs in the code and the code works for all the cases.

## How to Compile and Run

1. Create a new `build` directory after unzipping the contents
   1. The directory structure should look similar to this:
    ```.
├── CMakeLists.txt
├── Lab2.pdf
├── README.md
├── include
│   ├── Benes.hpp
│   ├── Delta.hpp
│   ├── Logging.hpp
│   ├── Omega.hpp
│   ├── Switch.hpp
│   └── SwitchNetwork.hpp
├── parser-lib
│   ├── CMakeLists.txt
│   ├── include
│   │   ├── Argument.hpp
│   │   ├── ArgumentInfo.hpp
│   │   ├── ArgumentType.hpp
│   │   ├── CommandLineParser.hpp
│   │   └── Logging.hpp
│   ├── src
│   │   ├── Argument.cpp
│   │   ├── ArgumentInfo.cpp
│   │   ├── CMakeLists.txt
│   │   └── CommandLineParser.cpp
│   └── test
│       ├── CMakeLists.txt
│       ├── cli.json
│       └── test_main.cpp
└── src
    ├── Benes.cpp
    ├── Delta.cpp
    ├── Omega.cpp
    ├── Switch.cpp
    ├── SwitchNetwork.cpp
    ├── main.cpp
    └── params.json
    ```
1. `cd build` into the build directory
2. Run `cmake ..` to generate the makefile
3. Run `make` to compile the code. This will generate the binary `SwitchNetwork`
   1. Please note that the binary requires the `params.json` file to be present in the same directory
4. Run `./SwitchNetwork -in infile.txt -sw Omega` using your own arguments!
