#pragma once

#include <string>
#include <fstream>
#include "Logging.hpp"
#include <vector>
#include "TrafficGenerator.hpp"
#include "Switch.hpp"

namespace Simulation{

#define SIMULATOR_LOGMODULE "Simulator"

    struct Config{
        int numPorts; // number of Ports in the switch
        int buffersize; // upto B fixed length packets can be held
        float packetgenprob;    // probability that a packet is generated per slot per input port
        QueueType qtype;   
        int maxslots;   // maximum number of slots for which the simulation should be run
        int K;  // how fast the back plane should be
        int L; // how many HOL packets should be used for the scheduling 
        std::string outputfilename;
    };

    class Simulator{

        public:
            Simulator(struct Config&& conf) : conf(conf), 
                tg(conf.numPorts, conf.packetgenprob), 
                sw(conf.numPorts, conf.buffersize, conf.qtype, conf.K, conf.L),
                delays(), portUses(conf.numPorts, 0) {};

            int Start(void); // start the simulation for max time and write the values to the files

        private:

            Config conf;
            TrafficGenerator tg;
            Switch sw;
            std::vector<int> delays;
            std::vector<int> portUses;

    };
}