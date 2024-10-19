#pragma once

#include <chrono>
#include <iostream>
#include <optional>
#include <queue>
#include <set>
#include <vector>
#include "Packet.h"

class Scheduler
{
public:

    Scheduler(int numSrc, std::vector<float> wts, int ticksize) : ticksize(ticksize), numSrc(numSrc), numPackets(numSrc, 0), weights(wts), flowFinishNumbers(numSrc, -1)
    {   
        roundNumber = 0.0;
        sumActiveWeights = 0.0;
        lastTrigger = std::nullopt;

        // initialise the set
        for(int i=0; i<numSrc; i++)
            sortedFlowFinishNumbers.insert({flowFinishNumbers[i], i});
    }

    void UpdateRoundNumber()
    {
        if(lastTrigger.has_value())
        {
            // check for one iterated deletion here

            const auto& timenow = std::chrono::steady_clock::now();
            double projectedRoundNumber;
            const auto& UpdateProjectedRoundNumber = [&projectedRoundNumber, &timenow, this]()
            {
                double durationInMillis = std::chrono::duration_cast<std::chrono::duration<double>>(timenow - lastTrigger.value()).count() * 1000 / ticksize;
                std::cout << "Duration is " << durationInMillis << "\n";
                // std::cout << "Projected Round Number is " << projectedRoundNumber << "\n";
                projectedRoundNumber = roundNumber + durationInMillis / sumActiveWeights;
            };
            UpdateProjectedRoundNumber();

            auto iter = sortedFlowFinishNumbers.begin();
            while(iter->first < roundNumber && iter != sortedFlowFinishNumbers.end())
                iter++;
            std::vector<std::pair<double, int>> toBeRemoved;

            while(iter!=sortedFlowFinishNumbers.end())
            {
                if(iter->first > projectedRoundNumber)
                    break;

                // time for iterated deletion
                toBeRemoved.push_back(*iter);
                sumActiveWeights -= weights[iter->second];

                UpdateProjectedRoundNumber();
            }

            // remove the toBeRemoved entries
            for(auto entry : toBeRemoved)
            {
                sortedFlowFinishNumbers.erase(sortedFlowFinishNumbers.find(entry));
            }

            roundNumber = projectedRoundNumber;
            lastTrigger = timenow;
        }
        else
        {
            // no need to do anything as there were no active flows before this
            roundNumber = 0.0;
            lastTrigger = std::chrono::steady_clock::now();
        }
    }

    void addPacket(int src, int pktlen)
    {
        // std::cout << "Source: " << src << " generated packet of length " << pktlen << "\n";
        numPackets[src]++;

        UpdateRoundNumber();
        std::cout << "Round# =" << roundNumber << "\n";
        
        // compute the finish number of the new packet
        Packet pkt{
            .sourceNumber = src,
            .length = pktlen,
            .enqTime = std::chrono::steady_clock::now()
        };

        pkt.finishNumber = std::max(roundNumber, flowFinishNumbers[src]) + static_cast<double>(pkt.length) / weights[src];

        if(flowFinishNumbers[src]<roundNumber)
        {
            // means earlier it was inactive
            sumActiveWeights += weights[src];
        }

        if(flowFinishNumbers[src]<pkt.finishNumber)
        {
            sortedFlowFinishNumbers.erase(sortedFlowFinishNumbers.find({flowFinishNumbers[src], src}));
            flowFinishNumbers[src] = pkt.finishNumber;
            sortedFlowFinishNumbers.insert({flowFinishNumbers[src], src});
        }

        // compute finish numbers here
        queue.push(std::move(pkt)); // to avoid copying
    }

    ~Scheduler()
    {
        // output the stats
        for(int i=0; i<numSrc; i++)
        {
            std::cout << numPackets[i] << " packets came from source #" << i << "\n"; 
        }
    }

private:

    int ticksize; // number of milliseconds in a tick
    int numSrc;
    double roundNumber;
    double sumActiveWeights;

    std::optional<std::chrono::steady_clock::time_point> lastTrigger;

    std::vector<int> numPackets;

    std::vector<float> weights; // stores the weights for every source, indexed by source number
    std::vector<double> flowFinishNumbers; // stores the finish numbers of all the sources, indexed by src numbers
    std::set<std::pair<double, int>> sortedFlowFinishNumbers;

    // now this will 
    std::priority_queue<Packet, std::vector<Packet>, PacketFinishComparator> queue;  // this has to be accessed in the order of finish number
    
};