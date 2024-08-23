# Virtual Circuit Switching Simulator

## Working

There are no bugs in the code and the code works for all the cases.

## How to Compile and Run

1. Create a new `build` directory after unzipping the contents
   1. The directory structure should look like this:
    ```
    ├── build
    ├── src
    ├── README.md
    ├── report.pdf
    ├── tests
    ├── results.csv
    ├── parser-lib
    ├── include
    ```
2. `cd build` into the build directory
3. Run `cmake ..` to generate the makefile
4. Run `make` to compile the code. This will generate the binary `NetworkSimulator`
   1. Please note that the binary requires the `params.json` file to be present in the same directory
5. Run `./NetworkSimulator -top topology.txt -conn connections.txt -rt routingfile.txt -ft forwardingfile.txt -path pathsfile.txt -flag hop -p 1` using your own arguments!

