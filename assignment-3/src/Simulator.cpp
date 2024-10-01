#include "Simulator.hpp"

namespace Simulation{

    int Simulator::Start(void)
    {
        for(int i=0; i<conf.maxslots; i++)
        {
            //// Generate the packet ////
            auto inpkts = tg.GeneratePackets(i);

            //// Switch the packets ////
            sw.AddPackets(inpkts);
            sw.ScheduleAndSwitch();

            //// Get the switched packets ////
            auto outpkts = sw.ConsumePackets();
            for(auto &pkt : outpkts)
                if(pkt.has_value())
                    pkt->endSlot = i;

            // update the delays
            for(const auto& pkt : outpkts)
                if(pkt.has_value()){
                    delays.push_back(pkt->endSlot - pkt->startSlot + 1);
                    std::cout << pkt.value() << "\n";
                }
        
        }
    }

}
