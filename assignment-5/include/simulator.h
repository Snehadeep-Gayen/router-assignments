#pragma once

#include <string>
#include <fstream>
#include "Logging.h"
#include <vector>
#include <bitset>

namespace Simulation{

using namespace Logging;

#define SIMULATOR_LOGMODULE "Simulator"

    struct Config{
        std::string rulesfilename;
        std::string inputfilename;
        std::string outputfilename;
    };

    struct NetIP{
        uint32_t ip;
        uint32_t mask;
        int bits;

        NetIP(std::string netmask_sting);
        NetIP(uint32_t ip, int maskbits);

        bool match(uint32_t ipaddr);

        static std::vector<bool> MakeVector(NetIP nip);
    };
    

    struct Rule{
        struct NetIP i1, i2;
    };

    class Simulator{

        public:
            Simulator(struct Config&& conf);

            void Start(void); // start the simulation for max time and write the values to the files

        private:

            Config conf;
            std::vector<Rule> rules;
            std::vector<std::pair<uint32_t, uint32_t>> tests;

    };
}