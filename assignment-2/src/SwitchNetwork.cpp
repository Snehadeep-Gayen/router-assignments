#include "SwitchNetwork.hpp"
#include "Benes.hpp"
#include "Omega.hpp"
#include "Delta.hpp"
#include "Switch.hpp"
#include <cassert>

namespace Simulation
{

    Simulator::Simulator(struct Config&& conf) : conf(conf) 
    {
        Logging::LOGI(SIMULATOR_LOGMODULE, "Setting up the Switch");

        // get the number of ports from the file
        std::ifstream inputfile(conf.inputfilename);
        assert(inputfile.is_open());
        int numPorts;
        inputfile >> numPorts;


        if(conf.switchType == "Benes")
            sw = std::make_unique<Switch::Benes>(Switch::Benes(numPorts));
        else if(conf.switchType == "Omega")
            sw = std::make_unique<Switch::Omega>(Switch::Omega(numPorts));
        else if(conf.switchType == "Delta")
            sw = std::make_unique<Switch::Delta>(Switch::Delta(numPorts));
        else
            throw std::runtime_error("Invalid Switch Type " + conf.switchType);

        std::cout << conf.switchType << "\n";

        Logging::LOGI(SIMULATOR_LOGMODULE, "Switch Setup done");
    };

    int Simulator::Start(void)
    {
        Logging::LOGI(SIMULATOR_LOGMODULE, "Starting the Switching");

        // get the number of inputs from the file
        std::ifstream inputfile(conf.inputfilename);
        assert(inputfile.is_open());

        int numPorts, numPackets;
        inputfile >> numPorts >> numPackets;
        std::vector<int> inputPackets(numPackets, -1);;
        for(auto &i : inputPackets) 
            inputfile >> i;

        sw->SwitchPackets(inputPackets);
        const auto& swConf = sw->GetConfigurations();
        Logging::LOGI(SIMULATOR_LOGMODULE, "Got the configuration of the switch after switching");
        sw->PrintConfig();
    
        return 0;
    }
}