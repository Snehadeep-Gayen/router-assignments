#pragma once

#include <cassert>
#include <deque>
#include <iostream>
#include <queue>
#include <set>
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
                    inputQs[i].push_back(pkts[i].value());
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
                        pktQ.pop_front();
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
                        outputQs[i].push_back(finalOutputs[i].value());
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
                        pktQ.pop_front();
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
                        outputQs[i].push_back(finalOutputs[i].value());
                }
            }
            else if(type==QueueType::CIOQ)
            {

                for(int backplane_iter=0; backplane_iter<speedup; backplane_iter++)
                {
                    // iSLIP algorithm: ref. DOI: 10.1109/90.769767
                    
                    // initially all (non-empty) input ports want to send their packets
                    std::vector<bool> inputQsatisfied(numPorts, false);
                    std::vector<bool> outputQsatisfied(numPorts, false);
                    for(int i=0; i<inputQs.size(); i++)
                        if(inputQs.empty())
                            inputQsatisfied[i] = true;

                    std::vector<int> finalAccepted(numPorts, -1);   // stores the final allocation of the packets

                    //// INITIALISE THE LAST-SENT POINTERS /////
                    std::vector<int> lastGrantedAndAccepted(numPorts, 0); // initial value doesn't affect 
                    std::vector<int> lastAccepted(numPorts, 0); // initial value doesn't matter much
                    
                    // start with the iterations
                    for(int iSLIP_iter=0; iSLIP_iter<numPorts; iSLIP_iter++)   // theoretically, upper bound is the numPorts
                    {
                        //////////////// REQUEST PHASE /////////////////

                        std::vector<std::set<int>> requests(numPorts); 

                        // every input port sends requests to the output ports, it has packets for (first L packets only)
                        for(int port=0; port<numPorts; port++)
                        {
                            if(inputQsatisfied[port])
                                continue;

                            for(int pktidx=0; pktidx < std::min(L, (int)inputQs[port].size()); pktidx++)
                            {
                                requests[inputQs[port][pktidx].outputPort].insert(port);
                            }
                        }

                        ////////////// GRANT PHASE /////////////////

                        // std::vector<int> selectedPort(numPorts, -1);
                        std::vector<std::vector<int>> grants(numPorts);

                        for(int port=0; port<numPorts; port++)
                        {
                            if(requests[port].empty() || outputQsatisfied[port])
                                continue;
                            auto closest = requests[port].lower_bound((lastGrantedAndAccepted[port]+1)%numPorts);
                            if(closest==requests[port].end())
                            {
                                // loop around and choose the first
                                closest = requests[port].begin();
                            }

                            // now closest is the candidate for grant
                            // selectedPort[port] = *closest;
                            grants[*closest].push_back(port);
                        }

                        /////////// ACCEPT PHASE /////////////

                        bool noChange = true;

                        for(int port=0; port<numPorts; port++)
                        {
                            if(grants[port].empty())
                                continue;
                            auto closest = std::lower_bound(grants[port].begin(), grants[port].end(), (lastGrantedAndAccepted[port]+1)%numPorts);
                            if(closest==grants[port].end())
                                closest = grants[port].begin();

                            // now accept the next highest priority connection (Round Robin)
                            finalAccepted[port] = *closest;
                            lastGrantedAndAccepted[*closest] = port;
                            lastAccepted[port] = *closest;
                            
                            // set the output and input ports to satisfied
                            inputQsatisfied[port] = true;
                            outputQsatisfied[*closest] = true;

                            noChange = false;
                        }

                        if(noChange)
                            break;
                    }

                    std::vector<std::optional<Packet>> finalOutputs;
                    for(int port=0; port<numPorts; port++)
                    {
                        if(finalAccepted[port]==-1)
                        {
                            finalOutputs.push_back(std::nullopt);
                            continue;
                        }

                        // otherwise check the first packet for that port and pick it
                        bool found = false;
                        for(int i=0; i<inputQs[port].size(); i++)
                        {
                            if(inputQs[port][i].outputPort==finalAccepted[port])
                            {
                                // found the needed packet
                                finalOutputs.push_back(inputQs[port][i]);
                                inputQs[port].erase(inputQs[port].begin()+i);   // remove that packet from the input queue

                                found = true;
                            }
                        }

                        assert(found);
                    }

                    // send the packets to the output ports
                    for(int i=0; i<numPorts; i++)
                    {
                        if(finalOutputs[i].has_value() && outputQs[i].size()<bufferSize)
                            outputQs[i].push_back(finalOutputs[i].value());
                    }
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
                    q.pop_front();
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
        std::vector<std::deque<Packet>> inputQs;

        // output queues
        std::vector<std::deque<Packet>> outputQs;
};