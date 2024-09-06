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
        std::vector<bool> SwitchPackets(const std::vector<int>& outputPorts);

    private:

// returns the position after a perfect shuffle with `numPorts` cards
        int GetShufflePosition(int posBefore);

        // returns the position that would have lead to this position after a shuffle of `numPorts` cards
        int GetInverseSufflePosition(int posAfter);

        // Determines first switch contention and last switch contention and decides the halves that need to be routed
        // To deal with Empty input ports, put -1 in the corresponding entries of the outputPorts
        void DetermineConfiguration(const std::vector<int>& outputPorts);

        void TwoColorDFS(int node, std::vector<int>& color, const std::vector<std::vector<int>>& adj);

        /* 

        Flow of routing packets

        1. SwitchPackets(OutputPorts)

        2. Calls DistributeHalves, to determine which of the packets go to the top half and which go to the bottom half

        3. Instantiate a Benes (n-1) network and calls switchPackets in this object to record the configuration of the top and bottom halves of the switches

        4. Prepends & Appends the Configuration of the first & last level of switches to the output configuration of the two halves
        
        */

};

}