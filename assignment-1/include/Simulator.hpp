#pragma once

namespace Simulation{

    struct Config{
        std::string topofile;
        std::string connfile;
        std::string rtabfile;
        std::string fwdfile;
        bool hop;
        bool pess;
    };

    class Simulator{

        public:
            Simulator(struct Config& conf) : conf(conf), control(conf.pess);

        private:
            struct Config conf;
            Controller control;

    };
}