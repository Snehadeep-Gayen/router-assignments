#pragma once

#include <string>
#include <vector>

namespace Switch {

#define SWITCH_LOGMODULE    "Switch"

    class Switch
    {
        public:

            enum SwitchConfig
            {
                CROSS,
                THROUGH
            };

            // each switch has a fixed number of ports (and # input ports = # output ports)
            Switch(int numPorts) : numPorts(numPorts) {};

            // routes the packets from input ports 0, 1, 2, ... vec.size() to the respective output ports
            // returns a true/false for each packet indicating whether it was dropped or not
            virtual std::vector<bool> switchPackets(std::vector<int> outputPorts) = 0;

            // returns the configuration (cross or through for each 2x2 switch in the network
            // true means cross, false means
            virtual std::vector<std::vector<SwitchConfig>> getConfigurations(void) = 0;
        
        private:

            int numPorts;
    };
}