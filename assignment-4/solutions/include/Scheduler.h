#pragma once

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <optional>
#include <queue>
#include <set>
#include <vector>
#include "Packet.h"
// #include "Simulator.h"

class Scheduler
{
public:

    Scheduler(int numSrc, std::vector<float> wts, int ticksize, double processingCapacity) : processingCapacity(processingCapacity), ticksize(ticksize), numSrc(numSrc), numPackets(numSrc, 0), weights(wts), flowFinishNumbers(numSrc, -1)
    {   
        roundNumber = 0.0;
        sumActiveWeights = 0.0;
        lastTrigger = std::nullopt;
    }

    void UpdateRoundNumber()
    {
        if(lastTrigger.has_value())
        {
            // check if all flows are inactive
            bool all_inactive = std::all_of(flowFinishNumbers.begin(), flowFinishNumbers.end(), [this](double finishNumber){ return finishNumber <= roundNumber; });
            if(all_inactive)
            {
                roundNumber = 0;
                if(std::abs(sumActiveWeights)>1e-3)
                {
                    std::cout << sumActiveWeights << "\n";
                    exit(0);
                }
                sumActiveWeights = 0.0;
                flowFinishNumbers = std::vector<double>(numSrc, -1);
                return;
            }


            // check for one iterated deletion here
            const auto& timenow = std::chrono::steady_clock::now();
            double projectedRoundNumber;
            const auto& UpdateProjectedRoundNumber = [&projectedRoundNumber, &timenow, this]()
            {
                double durationInMillis = std::chrono::duration_cast<std::chrono::duration<double>>(timenow - lastTrigger.value()).count() * 1000 / ticksize;
                if(printStats==0)
                    std::cout << "Duration is " << durationInMillis << "\n";
                // std::cout << "Projected Round Number is " << projectedRoundNumber << "\n";
                projectedRoundNumber = roundNumber + durationInMillis / (std::abs(sumActiveWeights)<1e-3?(INFINITY):sumActiveWeights);
            };
            UpdateProjectedRoundNumber();

            // sorted according to finish numbers. Second value is the weight of the flow
            double sumWeights = 0.0;
            std::vector<std::pair<double, double>> sortedFlows;
            for(int src=0; src<numSrc; src++)
                if(flowFinishNumbers[src]>roundNumber){
                    sumWeights += weights[src];
                    sortedFlows.push_back({flowFinishNumbers[src], weights[src]});
                }
            std::sort(sortedFlows.begin(), sortedFlows.end());
            if(std::abs(sumWeights - sumActiveWeights)>1e-3)
            {
                std::cout << "Difference in weights\n";
                std::cout << sumActiveWeights << " " << sumWeights << "\n";
                exit(0);
            }

            // iterate and delete
            double startingRoundNumber = roundNumber;
            for(auto [finishNumber, weight] : sortedFlows)
            {
                if(finishNumber<projectedRoundNumber)
                {
                    // delete this flow
                    std::chrono::microseconds microTime (static_cast<int64_t>(((finishNumber - roundNumber) * sumActiveWeights * ticksize * 1000)));
                    std::chrono::steady_clock::time_point inactiveTime = lastTrigger.value() + microTime;
                    roundNumber = finishNumber;
                    lastTrigger = inactiveTime;
                    sumActiveWeights -= weight;

                    assert(sumActiveWeights>=-1e-3);
                }
                else
                {
                    break;
                }
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

        if(++printStats==1)
        {
            std::cout << "Round# =" << roundNumber << "\n";
            std::cout << "SumWeights =" << sumActiveWeights << "\n";
            for(int i=0; i<numSrc; i++)
            {
                std::cout << "Finish Number for source#" << i <<  " is " << flowFinishNumbers[i] << "\n";
            }
            printStats = 0;
        }
        UpdateRoundNumber();

        // compute the finish number of the new packet
        Packet pkt{
            .sourceNumber = src,
            .length = pktlen,
            .enqTime = std::chrono::steady_clock::now()
        };

        pkt.finishNumber = std::max(roundNumber, flowFinishNumbers[src]) + pkt.length / (processingCapacity * weights[src]);

        if(flowFinishNumbers[src]<roundNumber)
        {
            // means earlier it was inactive
            sumActiveWeights += weights[src];
        }

        flowFinishNumbers[src] = std::max(flowFinishNumbers[src], pkt.finishNumber);    // Max is unnecessary here, it will always be true

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
    double processingCapacity;
    int numSrc;
    double roundNumber;
    double sumActiveWeights;

    int printStats = 0;

    std::optional<std::chrono::steady_clock::time_point> lastTrigger;

    std::vector<int> numPackets;

    std::vector<float> weights; // stores the weights for every source, indexed by source number
    std::vector<double> flowFinishNumbers; // stores the finish numbers of all the sources, indexed by src numbers

    // now this will 
    std::priority_queue<Packet, std::vector<Packet>, PacketFinishComparator> queue;  // this has to be accessed in the order of finish number
    
};