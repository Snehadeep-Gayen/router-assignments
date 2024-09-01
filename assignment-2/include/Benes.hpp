#pragma once


#include "Switch.hpp"

namespace Switch{

#define BENES_LOGGING "Benes"

class Benes : public Switch
{
    public:

        Benes(int numPorts) : Switch(numPorts) {};

        // routes the packets from input ports 0, 1, 2, ... vec.size() to the respective output ports
        // returns a true/false for each packet indicating whether it was dropped or not
        std::vector<bool> switchPackets(std::vector<int> outputPorts);

        // returns the configuration (cross or through for each 2x2 switch in the network
        // true means cross, false means
        std::vector<std::vector<SwitchConfig>> getConfigurations(void);


    private:

        // std::vector<std::vector<


};

}