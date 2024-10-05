#include "Simulator.hpp"
#include "Switch.hpp"

namespace Simulation{

    int Simulator::Start(void)
    {
        int total_pkts_generated = 0;
        for(int i=0; i<conf.maxslots; i++)
        {
            //// Generate the packet ////
            auto inpkts = tg.GeneratePackets(i);
            for(auto pkt_opt : inpkts)
                if(pkt_opt.has_value())
                    total_pkts_generated++;

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

        // calculate the stats
        double pkt_delay = 0;
        for(auto delay : delays){
            std::cout << delay << ",";
            pkt_delay += delay; 
        }
        double avg_plt_delay = pkt_delay /= delays.size();

        double drop_prob = 1 - delays.size() / (double) total_pkts_generated;

        double link_utilisation = delays.size() / ((double) conf.maxslots * conf.numPorts);

        std::ofstream file(conf.outputfilename, std::ofstream::app);
        assert(file.is_open());

        file << conf.numPorts << "," << conf.packetgenprob << ",";
        
        if(conf.qtype == QueueType::CIOQ)
        {
            file << conf.L << "," << conf.K << ",";
        } 

        file << conf.qtype << "," << avg_plt_delay << "," << link_utilisation;

        if(conf.qtype == QueueType::CIOQ)
        {
            file << "," << drop_prob;
        } 

        file << "\n";

        return 0;
    }

}
