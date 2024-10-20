#pragma once

#include <cassert>
#include <chrono>
#include <memory>
#include <string>
#include <fstream>
#include "Logging.h"
#include <vector>
#include "PacketGen.h"
#include "Scheduler.h"

namespace Simulation{

#define SIMULATOR_LOGMODULE "Simulator"
#define SETUP_TIME (2)
// number of milliseconds in one tick
#define TICKSIZE (10)

    struct Config
    {
        int n;  // number of sources
        int t;  // total simulation time
        int c;  // pkt processing capacity of the output link
        int b;  // queue capacity
        std::vector<PacketGen::PacketSourceConfig> pktSrcConfigs;   // configuration of each packet
        std::vector<float> weights; // weights of each packet source
        std::string outputfilename; // output file to write to
    };

    class Simulator
    {
        public:
            Simulator(struct Config&& conf) : conf(conf) {};

            void Start(void) 
            {
                // initial setup
                std::shared_ptr<Scheduler> schptr = std::make_shared<Scheduler>(conf.n, conf.weights, TICKSIZE, conf.c);
                PacketGen::PacketSource::sched = schptr;
                assert(conf.n == conf.pktSrcConfigs.size());
                std::cout << "Starting Packet Sources\n";

                for(int i=0; i<conf.n; i++)
                {
                    std::cout << "Starting Packet Source " << i << "\n";
                    auto pktsrc = new PacketGen::PacketSource(conf.pktSrcConfigs[i]);
                }

                // for(auto& pktsrc : pktsrcs)
                //     pktsrc.join();

                std::cout << conf.t << " is simulation time\n";
                std::this_thread::sleep_until(std::chrono::steady_clock::now() + std::chrono::milliseconds((SETUP_TIME * 1000 + conf.t * TICKSIZE)));   // remove this voodoo constant

            }; // start the simulation for max time and write the values to the files

        private:

            Config conf;
    };
}