#include "CommandLineParser.hpp"
#include "Logging.h"
#include <chrono>
#include <string>
#include <cassert>
#include "PacketGen.h"
#include "Simulator.h"

#define CONFIG_LOGM ("Simulator Config")

struct Simulation::Config prepareConfig(std::unordered_map<std::string, CommandLine::Argument> argMap);

int main(int argc, char* argv[])
{
    const auto& argMap = CommandLine::ArgumentMapFactory::parse(
        argc,
        argv,
        "params.json");

    // Create the simulator
    Simulation::Simulator sim(prepareConfig(argMap));
    sim.Start();

    return 0;
}


struct Simulation::Config prepareConfig(std::unordered_map<std::string, CommandLine::Argument> argMap)
{
    struct Simulation::Config cfg;

    Logging::LOGI(CONFIG_LOGM, "Starting argMap to Config conversion");
    
    std::optional<CommandLine::ArgumentType> argVal;

    Logging::LOGI(CONFIG_LOGM, "Checking first argument - out");
	argVal = argMap.at("out").getArg();
	assert(argVal.has_value());
	cfg.outputfilename = std::get<std::string>(argVal.value());

    Logging::LOGI(CONFIG_LOGM, "Checking second argument - in");
	argVal = argMap.at("in").getArg();
	assert(argVal.has_value());
	std::string infilename = std::get<std::string>(argVal.value());
    
    // extract the relevant info from the file
    std::ifstream infile(infilename);
    if(!infile.is_open())
    {
        std::cerr << "Cannot find file " << infilename << "\n";
        exit(0);
    }

    char var;
    char eq;

    // Capture N={num}
    infile >> var >> eq >> cfg.n;

    // Capture T={num}
    infile >> var >> eq >> cfg.t;
    std::cout << std::string(60, '#') << std::endl;
    std::cout << "Approximate time needed for simulation: " << (cfg.t * TICKSIZE)/1000 + SETUP_TIME + 10 /* error margin */ << " seconds\n";
    std::cout << std::string(60, '#') << std::endl;

    // Capture c={num}
    infile >> var >> eq >> cfg.c;

    // Capture b={num}
    infile >> var >> eq >> cfg.b;

    for(int i=0; i<cfg.n; i++)
    {
        // TODO: Set the start time to something reasonable (either here or somewhere else)
        PacketGen::PacketSourceConfig psc;
        float weight;

        psc.sourceNumber = i;
        psc.simDuration = cfg.t;
        psc.simStart = std::chrono::steady_clock::now() + std::chrono::seconds(SETUP_TIME); // CHANGE THIS
        
        infile >> psc.p >> psc.lmin >> psc.lmax >> weight >> psc.startFraction >> psc.endFraction;

        cfg.weights.push_back(weight);

        cfg.pktSrcConfigs.push_back(psc);
    }


    Logging::LOGI(CONFIG_LOGM, "Populated argMap to Config");

    return cfg;
}