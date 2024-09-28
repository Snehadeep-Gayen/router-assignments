#include "Delta.hpp"
#include "Logging.hpp"
#include <cassert>

namespace Switch{

std::vector<bool> Delta::SwitchPackets(const std::vector<int>& outputPorts)
{
    Logging::LOGI(DELTA_LOGGING, "Starting Switching for Delta Switch");

    std::vector<std::vector<Switch::SwitchConfig>> configs;
    std::vector<int> ports = outputPorts;
    while(ports.size() != numPorts)
        ports.push_back(-1);
    
    // Should there be an input shuffle to remove contention?
    Switch::Shuffle(ports);
    for(int layer=1; layer<=portLength; layer++)
    {
        configs.push_back(Switch::SwitchPackets(ports, layer));
        if(layer!=portLength)
            Butterfly(ports, layer);

        std::string output;
        for(auto i : configs.back())
            output += STR(i) + " ";
        Logging::LOGI(DELTA_LOGGING, "At layer " + STR(layer) + " Switch config is " + output);
        Logging::LOGI(DELTA_LOGGING, "Done with switching for layer " + STR(layer));
    }
    this->configs = configs;

    Logging::LOGI(DELTA_LOGGING, "Done Switching for Delta Switch");
    return std::vector<bool>(numPorts, true); // TODO: Change this
}

void Delta::Butterfly(std::vector<int>& outputPorts, int layer)
{
    std::vector<int> newOutputPorts(numPorts, -1);

    int blockSize = (1<<(portLength-layer));

    bool currentHalf = false; // false means upper half, corresponding to 0 bit
    for(int portStart=0; portStart<numPorts; portStart+=blockSize)
    {
        for(int port=portStart; port-portStart<blockSize; port++)
        {
            bool bit = Switch::GetIthBit(port, portLength - layer + 1);
            int newPort = (port + (bit != currentHalf) * (blockSize-1)) % numPorts;

            Logging::LOGI(DELTA_LOGGING, "Packet at port " + STR(port) + " got butterflown to port " + STR(newPort));

            newOutputPorts[newPort] = outputPorts[port];
        }
        currentHalf = (currentHalf == false); // time for processing the next half
    }

    outputPorts = newOutputPorts;
}

}