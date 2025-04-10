#pragma once

#include <string>
#include <fstream>
#include <memory>
#include "Logging.hpp"
#include "Switch.hpp"

namespace Simulation{

#define SIMULATOR_LOGMODULE          "Simulator"

    struct Config{
        std::string inputfilename;
        std::string switchType;
    };

    class Simulator{

        public:
            Simulator(struct Config&& conf);
            int Start(void);

        private:

            struct Config conf;
            std::unique_ptr<Switch::Switch> sw;
    };
}