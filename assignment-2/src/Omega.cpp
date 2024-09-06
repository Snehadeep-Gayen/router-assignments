#include "Omega.hpp"
#include <cassert>

namespace Switch{

std::vector<bool> Omega::SwitchPackets(const std::vector<int>& outputPorts)
{
    Logging::LOGI(OMEGA_LOGGING, "Starting Switching for Omega Switch");

    std::vector<std::vector<Switch::SwitchConfig>> configs;
    std::vector<int> ports = outputPorts;
    while(ports.size()<numPorts)
        ports.push_back(-1);
    for(int layer=1; layer<=portLength; layer++)
    {
        Shuffle(ports);
        configs.push_back(Switch::SwitchPackets(ports, layer));

        std::string output;
        for(auto i : configs.back())
            output += STR(i) + " ";
        Logging::LOGI(OMEGA_LOGGING, "At layer " + STR(layer) + " Switch config is " + output);
        Logging::LOGI(OMEGA_LOGGING, "Done with switching for layer " + STR(layer));
    }
    this->configs = configs;
    return std::vector<bool>(numPorts, true); // TODO: Change this
}

}