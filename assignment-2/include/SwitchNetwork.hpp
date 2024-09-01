#pragma once

#include <string>
#include <fstream>
#include "Logging.hpp"

namespace Simulation{

#define SIMULATOR_LOGMODULE          "Simulator"

    struct Config{
        std::string inputfilename;
        std::string switchType;
    };

    class Simulator{

        public:
            Simulator(struct Config&& conf) : conf(conf) {};
            int Start(void);

        private:

            struct Config conf;
            // Switch::Switch sw;
    };
}