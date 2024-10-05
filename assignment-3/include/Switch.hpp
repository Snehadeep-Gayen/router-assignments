#pragma once

#include <cassert>
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <vector>
#include <optional>
#include <algorithm>
#include "Packet.hpp"

enum QueueType{
    NOQ,
    INQ,
    CIOQ
};

class Switch
{
    public:
        Switch(int numPorts, int bufferSize, QueueType type, int speedup, int L) : 
            numPorts(numPorts), bufferSize(bufferSize), type(type), speedup(speedup), L(L),
            inputQs(numPorts), outputQs(numPorts)
        {
            
        };

        // adds the packets to the input queue and returns the number of dropped packets
        int AddPackets(std::vector<std::optional<Packet>> pkts)
        {
            assert(pkts.size()==numPorts);

            int pktsDropped = 0;
            for(int i=0; i<numPorts; i++)
            {
                if(pkts[i].has_value() && inputQs[i].size()<bufferSize)
                {
                    inputQs[i].push(pkts[i].value());
                }
                else
                {
                    // one packet is dropped
                    pktsDropped++;
                }
            }

            return numPorts;
        }

        void ScheduleAndSwitch()
        {
            // performs one round of scheduling the packets and switches the respective packets to the corresponding output ports
            if(type == QueueType::NOQ)
            {
                std::vector<std::vector<Packet>> outpkts(numPorts);
                std::for_each(inputQs.begin(), inputQs.end(), [&outpkts](auto& pktQ)
                {
                    if(!pktQ.empty())
                    {
                        outpkts[pktQ.front().outputPort].push_back(pktQ.front());
                        pktQ.pop();
                    }
                    assert(pktQ.empty());
                });

                // choose a random packet and drop others
                std::vector<std::optional<Packet>> finalOutputs;
                for(auto& pkts : outpkts)
                {
                    if(pkts.size()>0)
                        finalOutputs.push_back(pkts[random() % pkts.size()]);
                    else
                        finalOutputs.push_back(std::nullopt); // push null_opt instead
                }

                // send the packets to the output ports
                for(int i=0; i<numPorts; i++){
                    assert(outputQs.empty());
                    if(finalOutputs[i].has_value() && outputQs[i].size()<bufferSize)
                        outputQs[i].push(finalOutputs[i].value());
                }
            }
            else if(type == QueueType::INQ)
            {
                std::vector<int> pktcnt(numPorts, 0);
                std::for_each(inputQs.begin(), inputQs.end(), [&pktcnt](const auto& pktQ){
                    if(!pktQ.empty())
                        pktcnt[pktQ.front().outputPort]++;
                });

                // choose a random packet and drop others
                for(auto& cnt : pktcnt)
                    if(cnt>0){
                        // cnt = random() % cnt;
                        cnt=0;  // drop the first packet always
                    }

                std::vector<std::optional<Packet>> finalOutputs;
                std::for_each(inputQs.begin(), inputQs.end(), [&pktcnt, &finalOutputs](auto& pktQ){
                    if(!pktQ.empty() && pktcnt[pktQ.front().outputPort]--==0)  // beware of loop around case
                    {
                        finalOutputs.push_back(pktQ.front());
                        pktQ.pop();
                    }
                    else
                    {
                        finalOutputs.push_back(std::nullopt);
                    }
                });

                // send the packets to the output ports
                for(int i=0; i<numPorts; i++){
                    assert(outputQs.empty());
                    if(finalOutputs[i].has_value() && outputQs[i].size()<bufferSize)
                        outputQs[i].push(finalOutputs[i].value());
                }
            }
        }

        // removes one packet from each port and returns it
        std::vector<std::optional<Packet>> ConsumePackets(void)
        {
            std::vector<std::optional<Packet>> ans;
            for(auto &q : outputQs)
            {
                if(q.empty())
                    ans.push_back(std::nullopt);
                else
                {
                    ans.push_back(q.front());
                    q.pop();
                }
            }
            return ans;
        }

    private:

        // config parameters
        int numPorts;
        int bufferSize;
        QueueType type;
        int speedup;
        int L;

        // internal stuff


        // input queues
        std::vector<std::queue<Packet>> inputQs;

        // output queues
        std::vector<std::queue<Packet>> outputQs;
};