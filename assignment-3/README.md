# Packet Scheduling Simulator

## Working

There are no bugs in the code and the code works for all the cases.

## How to Compile and Run

1. Create a new `build` directory after unzipping the contents
   1. The directory structure should look like this:
    ```
        .
        ├── CMakeLists.txt
        ├── include
        ├── Lab3-Q.pdf
        ├── parser-lib
        ├── README.md
        ├── CS21B078_Report.pdf
        ├── src
        ├── test
        └── The_iSLIP_scheduling_algorithm_for_input-queued_switches.pdf
    ```
2. `cd build` into the build directory
3. Run `cmake ..` to generate the makefile
4. Run `make` to compile the code. This will generate the binary `routing`
   1. Please note that the binary requires the `params.json` file to be present in the same directory
5. Run `/routing -N switchportcount -B buffersize -p packetgenprob -q NOQ | INQ | CIOQ -K knockout -L inpq -o outputfile -T maxslots` using your own arguments!

