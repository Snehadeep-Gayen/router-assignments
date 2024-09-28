#include "Omega.hpp"
#include <cassert>

namespace Switch{

std::vector<bool> Omega::SwitchPackets(const std::vector<int>& outputPorts)
{
    std::vector<std::vector<Switch::SwitchConfig>> configs;
    std::vector<int> ports = outputPorts;
    while(ports.size()<numPorts)
        ports.push_back(-1);
    for(int layer=1; layer<=portLength; layer++)
    {
        Shuffle(ports);
        configs.push_back(Switch::SwitchPackets(ports, layer));
    }
    this->configs = configs;
    return std::vector<bool>(numPorts, true); // TODO: Change this
}

}