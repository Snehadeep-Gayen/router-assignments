#include <iostream>
#include <chrono>
#include <iomanip>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include "Scheduler.h"

using simtime = std::chrono::time_point<std::chrono::steady_clock>;

// number of milliseconds in one tick
#define TICKSIZE 10000

namespace PacketGen
{
    struct PacketSourceConfig
    {
        int sourceNumber; // unique id of the source
        int simDuration;  // simulation time
        int p;  // number of packets generated per unit time
        int lmin; // minimum length of the packet
        int lmax; // maximum length of the packet
        // length of the packet will be uniformly distributed between min and max length
        // int w;  // weight of this source, scheduler knows the weights of every guy to avoid faking
        double startFraction; // time at which this generator should start (reported as the fraction of the total simulation time)
        double endFraction; // time at which this generator should end (reported as the fraction of the total simulation time)

        // simulation start time
        std::chrono::time_point<std::chrono::steady_clock> simStart;
    };

    class PacketSource
    {
    public:

        // Check how to handle start of the the threads at the same time (maybe give the unix timestamp for start time)

        PacketSource(PacketSourceConfig conf) : conf(conf), defaultgen(), expgen(conf.p), lengen(conf.lmin, conf.lmax)
        {
            // initialise the random number generator
            // done

            // start the thread
            auto actualStart = conf.simStart + std::chrono::milliseconds(uint64_t(TICKSIZE * conf.startFraction * conf.simDuration));
            auto endTime = conf.simStart + std::chrono::milliseconds(uint64_t(TICKSIZE * conf.endFraction * conf.simDuration));
            
            // Start the packet generation thread
            pktThread = std::thread(&PacketSource::PacketGenThread, this, actualStart, endTime);
        }

        void join()
        {
            if(pktThread.joinable())
            {
                pktThread.join();
            }
            else 
            {
                std::cerr << "Thread is not joinable\n";
            }
        }

    private:

        static void PacketGenThread(std::shared_ptr<PacketSource> pktsrc, const simtime startTime, const simtime endTime)
        {
            // sleep until start + starting time of the packet source
            std::this_thread::sleep_until(startTime);

            while(1)
            {
                // generate a random double
                double nextGen = pktsrc->expgen(pktsrc->defaultgen);

                // sleep for that much time
                std::this_thread::sleep_for(std::chrono::milliseconds(uint64_t(nextGen * TICKSIZE)));

                if(std::chrono::steady_clock::now()>=endTime)
                    break;  // stop packet generation

                // generate a random packet length
                int packetlen = pktsrc->lengen(pktsrc->defaultgen); 

                // acquire scheduler lock and invoke the scheduler
                schedmutex->lock();
                sched->addPacket(pktsrc->conf.sourceNumber, packetlen);  // tell the scheduler to do bookkeeping and add the packet to the queue
                schedmutex->unlock();
            }
            
        }

        const PacketSourceConfig conf;

        // scheduler related stuff
        static std::shared_ptr<std::mutex> schedmutex;
        static const std::shared_ptr<Scheduler> sched;

        // RNG related
        std::default_random_engine defaultgen;
        std::exponential_distribution<double> expgen;   // interpacket arrival time generator
        std::uniform_int_distribution<int> lengen; // packet length generator

        // Thread to generate packets
        std::thread pktThread;
    };

}