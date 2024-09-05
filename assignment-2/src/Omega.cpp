#include "Omega.hpp"
#include <cassert>

namespace Switch{

std::vector<bool> Omega::SwitchPackets(const std::vector<int>& outputPorts)
{
    std::vector<std::vector<Switch::SwitchConfig>> configs;
    std::vector<int> ports;
    for(int layer=1; layer!=portLength; layer++)
    {
        configs.push_back(Switch::SwitchPackets(ports, layer));
        if(numPorts!=2)
            Shuffle(ports);
    }
    this->configs = configs;
    return std::vector<bool>(numPorts, true); // TODO: Change this
}

void Omega::Shuffle(std::vector<int>& outputPorts)
{
    std::vector<int> shuffledPorts(numPorts, -1);
    for(int oldPort=0; oldPort<numPorts; oldPort++)
    {
        int newPort = (oldPort*2)%numPorts;
        shuffledPorts[newPort] = outputPorts[oldPort];
    }
    outputPorts = shuffledPorts;
}

}