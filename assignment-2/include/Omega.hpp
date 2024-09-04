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
        std::vector<bool> SwitchPackets(const std::vector<int>& outputPorts);

    private:

        // performs one round of shuffle on the packets
        void Shuffle(std::vector<int>& outputPorts);
};

}