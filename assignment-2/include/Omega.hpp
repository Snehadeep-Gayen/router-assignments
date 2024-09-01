#pragma once

#include "Switch.hpp"

namespace Switch{

#define OMEGA_LOGGING "Omega"

class Omega : public Switch
{
    public: 

        Omega(int numPorts) : Switch(numPorts) {};

        // routes the packets from input ports 0, 1, 2, ... vec.size() to the respective output ports
        // returns a true/false for each packet indicating whether it was dropped or not
        std::vector<bool> switchPackets(std::vector<int> outputPorts);

        // returns the configuration (cross or through for each 2x2 switch in the network
        // true means cross, false means
        std::vector<std::vector<SwitchConfig>> getConfigurations(void);

    private:

        // determines the switch configuration assuming that the packets are 
        // self-routed using bit `i` in layer `i`
        std::vector<SwitchConfig> switchPackets(std::vector<int>& outputPorts, int layer);

        // performs one round of shuffle on the packets
        void shuffle(std::vector<int>& outputPorts);
};

}