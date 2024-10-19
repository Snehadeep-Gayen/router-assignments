#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include "Scheduler.h"


using simtime = std::chrono::time_point<std::chrono::steady_clock>;

namespace PacketGen
{
    struct PacketSourceConfig
    {
        int sourceNumber; // unique id of the source
        int simDuration;  // simulation time
        int p;  // number of packets generated per unit time
        int lmin; // minimum length of the packet
        int lmax; // maximum length of the packet
        double startFraction; // start time as a fraction of total simulation time
        double endFraction; // end time as a fraction of total simulation time
        std::chrono::time_point<std::chrono::steady_clock> simStart; // simulation start time
    };

    class PacketSource
    {
    public:
        // Constructor
        PacketSource(PacketSourceConfig conf);

        // Method to join the packet generation thread
        void join();

        // Scheduler-related variables
        static std::shared_ptr<std::mutex> schedmutex;
        static std::shared_ptr<Scheduler> sched;

    private:
        // Packet generation thread function
        static void PacketGenThread(PacketSource* pktsrc, const simtime startTime, const simtime endTime);

        const PacketSourceConfig conf;  // Configuration for the packet source

        // RNG related variables
        std::default_random_engine defaultgen;
        std::exponential_distribution<double> expgen;
        std::uniform_int_distribution<int> lengen;

        // Thread to generate packets
        std::thread pktThread;
    };
}
