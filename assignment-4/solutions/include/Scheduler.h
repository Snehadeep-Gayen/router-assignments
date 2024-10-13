#include <iostream>
#include <vector>

class Scheduler
{
public:
    void addPacket(int src, int pktlen);

private:
    std::vector<int> prios; // stores the weights for every source, indexed by source number
    std::vector<int> finishNumbers; // stores the finish numbers of all the sources, indexed by src numbers

    // now this will 


    
};