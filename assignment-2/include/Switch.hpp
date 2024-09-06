#pragma once

#include <vector>
#include "Logging.hpp"

namespace Switch {

#define SWITCH_LOGMODULE    "Switch"

    class Switch
    {
        public:

            enum SwitchConfig
            {
                CROSS,
                THROUGH,
                ANYTHING
            };

            friend std::ostream& operator<<(std::ostream& os, const Switch::SwitchConfig& sw);

            // each switch has a fixed number of ports (and # input ports = # output ports)
            Switch(int numPorts);

            // routes the packets from input ports 0, 1, 2, ... vec.size() to the respective output ports
            // returns a true/false for each packet indicating whether it was dropped or not
            virtual std::vector<bool> SwitchPackets(const std::vector<int>& outputPorts) = 0;

            // returns the configuration (cross or through for each 2x2 switch in the network
            // true means cross, false means
            std::vector<std::vector<SwitchConfig>> GetConfigurations(void);

            virtual ~Switch() = default;

        protected:

            // determines the switch configuration assuming that the packets are 
            // self-routed using bit `i` in layer `i`
            std::vector<SwitchConfig> SwitchPackets(std::vector<int>& outputPorts, int layer);

            // returns the Most significant ith bit of a number (i starts from 1 and goes to portLength)
            // assuming that the number has `log2(numPort)` number of bits in total
            bool GetIthBit(int num, int i);

            int numPorts;
            int portLength;
            std::vector<std::vector<SwitchConfig>> configs;
    };
}