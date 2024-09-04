#include "Switch.hpp"
#include <bitset>
#include <cassert>
#include <stdexcept>

namespace Switch{

    Switch::Switch(int numPorts) : numPorts(numPorts)
    {
        portLength = 0;
        for(int tempPorts=numPorts; tempPorts!=1; tempPorts/=2)
            portLength++;
    }


    std::vector<Switch::SwitchConfig> Switch::SwitchPackets(std::vector<int>& outputPorts, int layer)
    {
        std::vector<Switch::SwitchConfig> config;
        for(int i=0; i<outputPorts.size(); i+=2)
        {
            int firstPort = outputPorts[i];
            int secondPort = (i+1==outputPorts.size())? -1 : outputPorts[i+1];
            int mask = (1<<layer);

            firstPort = (firstPort==-1)? -1 : firstPort & mask;
            secondPort = (secondPort==-1)? -1 : secondPort & mask;

            if(firstPort==-1 && secondPort==-1)
                config.push_back(Switch::ANYTHING); // anything works
            else if(firstPort==-1)
                config.push_back(secondPort ? Switch::CROSS : Switch::THROUGH);
            else if(secondPort==-1)
                config.push_back(firstPort ? Switch::CROSS : Switch::THROUGH);
            else if(firstPort != secondPort)
                config.push_back(firstPort ? Switch::CROSS : Switch::THROUGH);
            else
                throw std::runtime_error("Contention in Omega Switch!!");
        }
        return config;
    }

    std::vector<std::vector<Switch::SwitchConfig>> Switch::GetConfigurations(void)
    {
        return this->configs;
    }

    bool Switch::GetIthBit(int num, int i)
    {
        assert(sizeof(num) == 4);
        std::bitset<32> bnum(num);
        return bnum[32 - portLength + i - 1] == 1;
    }

}