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
        Logging::LOGI(SWITCH_LOGMODULE, "2x2 Switching a set of packets");
        std::vector<Switch::SwitchConfig> config;
        
        for(int i=0; i<outputPorts.size(); i+=2)
        {
            int firstPort = outputPorts[i];
            int secondPort = (i+1==outputPorts.size())? -1 : outputPorts[i+1];

            firstPort = (firstPort==-1)? -1 : GetIthBit(firstPort, layer);
            secondPort = (secondPort==-1)? -1 : GetIthBit(secondPort, layer);

            if(firstPort==-1 && secondPort==-1)
                config.push_back(Switch::ANYTHING); // anything works
            else if(firstPort==-1)
                config.push_back(secondPort ? Switch::THROUGH : Switch::CROSS);
            else if(secondPort==-1)
                config.push_back(firstPort ? Switch::CROSS : Switch::THROUGH);
            else if(firstPort != secondPort)
                config.push_back(firstPort ? Switch::CROSS : Switch::THROUGH);
            else
            {
                Logging::LOGE(SWITCH_LOGMODULE, "Ports " + STR(i) + " and " + STR(i+1) + " contend, with desired outputs " 
                    + STR(firstPort) + "(" + STR(outputPorts[i]) + ") and " 
                    + STR(secondPort) + "(" + STR(outputPorts[i+1]) + ")");
                throw std::runtime_error("Contention in Switch!!");
            }

            if(config.back()==Switch::CROSS)
                std::swap(outputPorts[i], outputPorts[i+1]);
        
        }
        Logging::LOGI(SWITCH_LOGMODULE, "Done 2x2 Switching a set of packets");
        return config;
    }

    std::vector<std::vector<Switch::SwitchConfig>> Switch::GetConfigurations(void)
    {
        return this->configs;
    }

    bool Switch::GetIthBit(int num, int i)
    {
        assert(sizeof(num) == 4);
        return (std::bitset<32>(num)[portLength - i] == 1);
    }

    std::ostream& operator<<(std::ostream& os, const Switch::SwitchConfig& sw)
    {
        std::string conf;
        if(sw == Switch::SwitchConfig::CROSS)
            conf = "C";
        else if(sw == Switch::SwitchConfig::THROUGH)
            conf = "T";
        else
            conf = "T"; // default config
        os << conf;
        
        return os;
    }

    void Switch::PrintConfig(void)
    {
        // all the layers should have the same number of 2x2 switches
        int size = configs[0].size();
        for(const auto& vec : configs)
            assert(size == vec.size());

        for(int j=0; j<configs[0].size(); j++)
        {
            for(int i=0; i<configs.size()-1; i++)
                std::cout << configs[i][j] << " ";
            std::cout << configs[configs.size()-1][j];
            if(j != configs[0].size()-1)
                std::cout << "\n";
        }
    }

    void Switch::Shuffle(std::vector<int>& outputPorts)
    {
        std::vector<int> shuffledPorts(numPorts, -1);
        for(int oldPort=0; oldPort<numPorts; oldPort++)
        {
            int newPort = (oldPort*2)%numPorts + (oldPort >= numPorts/2);
            Logging::LOGI(SWITCH_LOGMODULE, "NewPort = "+STR(newPort)+", OldPort = "+STR(oldPort));
            shuffledPorts[newPort] = outputPorts[oldPort];
        }
        outputPorts = shuffledPorts;
    }

}