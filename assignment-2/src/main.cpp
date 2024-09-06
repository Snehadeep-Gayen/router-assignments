#include "SwitchNetwork.hpp"
#include "CommandLineParser.hpp"
#include "Logging.hpp"
#include <string>
#include <cassert>

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
    
    Logging::LOGI(CONFIG_LOGM, "Checking first argument - in ");
	std::optional<CommandLine::ArgumentType> argVal = argMap.at("in").getArg();
	assert(argVal.has_value());
	cfg.inputfilename = std::get<std::string>(argVal.value());

    Logging::LOGI(CONFIG_LOGM, "Checking first argument - type ");
	argVal = argMap.at("type").getArg();
	assert(argVal.has_value());
    std::string flag = std::get<std::string>(argVal.value());
    assert(flag=="Benes" || flag=="Delta" || flag=="Omega");
    cfg.switchType = flag;

    Logging::LOGI(CONFIG_LOGM, "Populated argMap to Config");

    return cfg;
}