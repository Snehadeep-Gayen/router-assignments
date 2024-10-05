#include "TrafficGenerator.hpp"

TrafficGenerator::TrafficGenerator(int numPorts, float probab) : numPorts(numPorts), prob(probab)
{
    // char* oldState = initstate(78, randState.data(), randState.size());
    srandom(78);
}

float TrafficGenerator::GenerateRandomStartDelay(void)
{
    return static_cast<float>(0.1f * random() / RAND_MAX);
}

std::vector<std::optional<Packet>> TrafficGenerator::GeneratePackets(int slot)
{
    std::vector<std::optional<Packet>> pkts(numPorts, std::optional<Packet>());
    
    for (int i = 0; i < numPorts; i++)
    {
        if (GenerateSinglePacket())
        {
            pkts[i] = Packet{
                .startSlot = slot,
                .startDelta = GenerateRandomStartDelay(),
                .inputPort = i,
                .outputPort = GenerateRandomOutputPort(),
                .uid = uid++
            };
        }
    }

    return pkts;
}

bool TrafficGenerator::GenerateSinglePacket(void)
{
    long double randomOutput = static_cast<long double>(random()) / RAND_MAX;
    return randomOutput <= prob;
}

int TrafficGenerator::GenerateRandomOutputPort(void)
{
    return random() % numPorts;
}
