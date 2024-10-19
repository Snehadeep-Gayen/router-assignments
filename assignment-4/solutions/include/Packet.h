#include <chrono>


struct Packet
{
    // metadata
    int sourceNumber;
    int length;
    double finishNumber;

    // time related data
    std::chrono::steady_clock::time_point enqTime;
    std::chrono::steady_clock::time_point deqTime;
    std::chrono::steady_clock::time_point egressTime;
    std::chrono::steady_clock::time_point creationTime;
};

struct PacketFinishComparator
{
    bool operator()(const Packet& lhs, const Packet& rhs)
    {
        return lhs.finishNumber < rhs.finishNumber;
    }
};