#pragma once

#include <ostream>

struct Packet
{
    // required data
    int startSlot;
    float startDelta;
    int endSlot;
    int inputPort;
    int outputPort;

    // other metadata
    long long int uid;

    // Overload the stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Packet& pkt)
    {
        // Print the Packet details
        os << "Packet { "
           << "uid: " << pkt.uid 
           << ", startSlot: " << pkt.startSlot 
           << ", startDelta: " << pkt.startDelta 
           << ", endSlot: " << pkt.endSlot 
           << ", inputPort: " << pkt.inputPort 
           << ", outputPort: " << pkt.outputPort 
           << " }";
        return os;
    }
};

