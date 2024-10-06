#include "CommandLineParser.hpp"
#include "Logging.h"
#include <string>
#include <cassert>
#include "Simulator.hpp"
#include "Switch.hpp"

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
    
    Logging::LOGI(CONFIG_LOGM, "Checking first argument - N ");
    std::optional<CommandLine::ArgumentType> argVal;
    if(argMap.contains("N"))
    {
        argVal = argMap.at("N").getArg();
        if(argVal.has_value())
            cfg.numPorts = std::get<int>(argVal.value());
    }
    else
        cfg.numPorts = 8;

    Logging::LOGI(CONFIG_LOGM, "Checking first argument - B ");
    if(argMap.contains("B"))
    {
        argVal = argMap.at("B").getArg();
        assert(argVal.has_value());
        cfg.buffersize = std::get<int>(argVal.value());
    }
    else
        cfg.buffersize = 10;

    Logging::LOGI(CONFIG_LOGM, "Checking first argument - L ");
	argVal = argMap.at("L").getArg();
	assert(argVal.has_value());
	cfg.L = std::get<int>(argVal.value());

    Logging::LOGI(CONFIG_LOGM, "Checking first argument - K ");
	argVal = argMap.at("K").getArg();
	assert(argVal.has_value());
	cfg.K = std::get<int>(argVal.value());

    Logging::LOGI(CONFIG_LOGM, "Checking first argument - T ");
    if(argMap.contains("T"))
    {
        argVal = argMap.at("T").getArg();
        assert(argVal.has_value());
        cfg.maxslots = std::get<int>(argVal.value());
    }
    else
        cfg.maxslots = 10000;

    Logging::LOGI(CONFIG_LOGM, "Checking first argument - p ");
    if(argMap.contains("p"))
    {
        argVal = argMap.at("p").getArg();
        assert(argVal.has_value());
        cfg.packetgenprob = std::get<double>(argVal.value());
    }
    else
        cfg.packetgenprob = 0.5;

    Logging::LOGI(CONFIG_LOGM, "Checking first argument - q ");

    if(argMap.contains("q"))
    {
        argVal = argMap.at("q").getArg();
        assert(argVal.has_value());
        std::string flag = std::get<std::string>(argVal.value());
        assert(flag=="NOQ" || flag=="INQ" || flag=="CIOQ");
        if(flag=="NOQ")
            cfg.qtype = QueueType::NOQ;
        else if(flag=="INQ")
            cfg.qtype = QueueType::INQ;
        else 
            cfg.qtype = QueueType::CIOQ;
    }
    else    
        cfg.qtype = QueueType::INQ;

    Logging::LOGI(CONFIG_LOGM, "Checking first argument - o ");
	argVal = argMap.at("o").getArg();
	assert(argVal.has_value());
    cfg.outputfilename = std::get<std::string>(argVal.value());

    Logging::LOGI(CONFIG_LOGM, "Populated argMap to Config");

    return cfg;
}