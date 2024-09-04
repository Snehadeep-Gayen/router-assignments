#pragma once

#include "Switch.hpp"

namespace Switch{

#define DELTA_LOGGING "Delta"

class Delta : public Switch
{
    public: 

        Delta(int numPorts) : Switch(numPorts) {};

        // routes the packets from input ports 0, 1, 2, ... vec.size() to the respective output ports
        // returns a true/false for each packet indicating whether it was dropped or not
        std::vector<bool> SwitchPackets(const std::vector<int>& outputPorts);

    private:

        // performs one round of butterfly network (at layer i) on the packets
        // layer 1 means all input ports are crossed
        // layer 2 means top half and bottom half are separately crossed
        void Butterfly(std::vector<int>& outputPorts, int layer);
};

}