#pragma once

#include <cstdlib>
#include <vector>
#include <array>
#include <optional>
#include "Packet.hpp"

class TrafficGenerator
{
public:
    TrafficGenerator(int numPorts, float probab);
    
    std::vector<std::optional<Packet>> GeneratePackets(int slot);

private:
    bool GenerateSinglePacket(void);
    int GenerateRandomOutputPort(void);
    float GenerateRandomStartDelay(void);

    int numPorts;
    float prob;
    std::array<char, 1024> randState;
    long long int uid = 1;
};
