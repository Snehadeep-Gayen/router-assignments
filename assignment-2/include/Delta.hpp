#pragma once

#include "Switch.hpp"

namespace Switch{

#define OMEGA_LOGGING "Delta"

class Delta : public Switch
{
    public: 

        Delta(int numPorts) : Switch(numPorts) {};

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

        // performs one round of butterfly network (at layer i) on the packets
        // layer 0 means all input ports are crossed
        // layer 1 means top half and bottom half are separately crossed
        void butterfly(std::vector<int>& outputPorts, int layer);
};

}