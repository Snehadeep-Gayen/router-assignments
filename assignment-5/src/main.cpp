#include "CommandLineParser.hpp"
#include "simulator.h"
#include <string>
#include <cassert>

#define CONFIG_LOGM ("Simulator Config")

using namespace Logging;

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


Simulation::Config prepareConfig(std::unordered_map<std::string, CommandLine::Argument> argMap)
{
    Simulation::Config cfg;

    Logging::LOGI(CONFIG_LOGM, "Starting argMap to Config conversion");
    
    Logging::LOGI(CONFIG_LOGM, "Checking first argument - N ");
    std::optional<CommandLine::ArgumentType> argVal;
    if(argMap.contains("p"))
    {
        argVal = argMap.at("p").getArg();
        if(argVal.has_value())
            cfg.rulesfilename = std::get<std::string>(argVal.value());
    }
    else
    {
        LOGE(CONFIG_LOGM, "Rulesfilename must be present");
        exit(0);
    }
    if(argMap.contains("i"))
    {
        argVal = argMap.at("i").getArg();
        if(argVal.has_value())
            cfg.inputfilename = std::get<std::string>(argVal.value());
    }
    else
    {
        LOGE(CONFIG_LOGM, "Rulesfilename must be present");
        exit(0);
    }
    if(argMap.contains("o"))
    {
        argVal = argMap.at("o").getArg();
        if(argVal.has_value())
            cfg.outputfilename = std::get<std::string>(argVal.value());
    }
    else
    {
        LOGE(CONFIG_LOGM, "Rulesfilename must be present");
        exit(0);
    }
    return cfg;
}