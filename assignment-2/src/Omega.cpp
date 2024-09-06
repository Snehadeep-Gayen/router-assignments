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

void Omega::Shuffle(std::vector<int>& outputPorts)
{
    std::stringstream portsDebug;
    for(auto i : outputPorts)
        portsDebug << i << " hehe ";
    Logging::LOGI(OMEGA_LOGGING, "Input packets before shuffle " + portsDebug.str());

    std::vector<int> shuffledPorts(numPorts, -1);
    for(int oldPort=0; oldPort<numPorts; oldPort++)
    {
        int newPort = (oldPort*2)%numPorts + (oldPort >= numPorts/2);
        Logging::LOGI(OMEGA_LOGGING, "NewPort = "+STR(newPort)+", OldPort = "+STR(oldPort));
        shuffledPorts[newPort] = outputPorts[oldPort];
    }
    outputPorts = shuffledPorts;

    portsDebug = std::stringstream("");
    for(auto i : outputPorts)
        portsDebug << i << " hehe ";
    Logging::LOGI(OMEGA_LOGGING, "Input packets after shuffle " + portsDebug.str());
}

}