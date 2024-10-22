#include "PacketGen.h"
#include "Simulator.h"

namespace PacketGen
{
    std::shared_ptr<std::mutex> PacketSource::schedmutex = std::make_shared<std::mutex>();
    Scheduler* PacketSource::sched = nullptr;

    // Constructor definition
    PacketSource::PacketSource(PacketSourceConfig conf) 
        : conf(conf), defaultgen(), expgen(conf.p), lengen(conf.lmin, conf.lmax)
    {
        // Calculate the actual start and end times for the packet generation
        auto actualStart = conf.simStart + std::chrono::milliseconds(uint64_t(TICKSIZE * conf.startFraction * conf.simDuration));
        auto endTime = conf.simStart + std::chrono::milliseconds(uint64_t(TICKSIZE * conf.endFraction * conf.simDuration));

        // Start the packet generation thread
        pktThread = std::thread(&PacketSource::PacketGenThread, this, actualStart, endTime);
    }

    // Join method to wait for the packet generation thread to finish
    void PacketSource::join()
    {
        if(pktThread.joinable())
            pktThread.join();
        else
            std::cerr << "Thread is not joinable\n";
    }

    // Definition of the packet generation thread function
    void PacketSource::PacketGenThread(PacketSource* pktsrc, const simtime startTime, const simtime endTime)
    {
        // Sleep until the actual start time
        std::this_thread::sleep_until(startTime);

        while (true)
        {
            // Generate the time for the next packet and sleep for that duration
            double nextGen = pktsrc->expgen(pktsrc->defaultgen);
            std::this_thread::sleep_for(std::chrono::milliseconds(uint64_t(nextGen * TICKSIZE)));

            // Stop if the current time has exceeded the end time
            if (std::chrono::steady_clock::now() >= endTime)
                break;

            // Generate a random packet length
            int packetlen = pktsrc->lengen(pktsrc->defaultgen);

            // Acquire scheduler lock and add the packet
            schedmutex->lock();
            sched->addPacket(pktsrc->conf.sourceNumber, packetlen);
            schedmutex->unlock();
        }
    }
}
