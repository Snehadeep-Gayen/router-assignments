#pragma once

#include <string>
#include <fstream>
#include "Logging.hpp"
#include "Controller.hpp"

namespace Simulation{

#define SIMULATOR_LOGMODULE          "Simulator"

    struct Config{
        std::string topofilename;
        std::string connfilename;
        std::string rtabfilename;
        std::string fwdfilename;
        std::string pathsfilename;
        bool hop;
        bool pess;
    };

    class Simulator{

        public:
            Simulator(struct Config&& conf) : conf(conf), control(conf.pess) {};
            int Start(void);

        private:

            int initialiseTopo(std::fstream& topofile);
            int compute(void);
            int addConnections(std::fstream& connfile);
            int writeOutputs(std::fstream& rtabfile, std::fstream& fwdtabfile, std::fstream& pathsfile);

            struct Config conf;
            Network::Controller control;
    };
}