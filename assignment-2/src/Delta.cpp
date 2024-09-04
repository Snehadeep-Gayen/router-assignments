#include "Delta.hpp"
#include "Logging.hpp"
#include "Switch.hpp"
#include <cassert>

namespace Switch{

std::vector<bool> Delta::SwitchPackets(const std::vector<int>& outputPorts)
{
    std::vector<std::vector<Switch::SwitchConfig>> configs;
    std::vector<int> ports;
    for(int layer=1; layer<portLength; layer++)
    {
        configs.push_back(Switch::SwitchPackets(ports, layer));
        if(numPorts!=2)
            Butterfly(ports, layer);
    }
    this->configs = configs;
    return std::vector<bool>(numPorts, true); // TODO: Change this
}

void Delta::Butterfly(std::vector<int>& outputPorts, int layer)
{
    std::vector<int> newOutputPorts(numPorts, -1);

    int blockSize = (1<<(portLength-layer));
    Logging::LOGI(DELTA_LOGGING, "Blocksize is " + STR(blockSize) + " for layer " + STR(layer));

    bool currentHalf = false; // false means upper half, corresponding to 0 bit
    for(int portStart=0; portStart<numPorts; portStart+=blockSize)
    {
        for(int port=portStart; port-portStart<blockSize; port++)
        {
            if(port==-1)
                continue;
            bool bit = Switch::GetIthBit(outputPorts[port], layer);
            int newPort = (port + (bit == currentHalf) * blockSize) % numPorts;

            Logging::LOGI(DELTA_LOGGING, "Packet at port " + STR(port) + " got butterflown to port " + STR(newPort));

            newOutputPorts[newPort] = outputPorts[port];
        }
        currentHalf = (currentHalf == false); // time for processing the next half
    }

    outputPorts = newOutputPorts;
}

}