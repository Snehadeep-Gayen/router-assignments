#include "Simulator.hpp"
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

	std::optional<CommandLine::ArgumentType> argVal = argMap.at("top").getArg();
	assert(argVal.has_value());
	cfg.topofilename = std::get<std::string>(argVal.value());

	argVal = argMap.at("conn").getArg();
	assert(argVal.has_value());
	cfg.connfilename = std::get<std::string>(argVal.value());

	argVal = argMap.at("rt").getArg();
	assert(argVal.has_value());
	cfg.rtabfilename = std::get<std::string>(argVal.value());

	argVal = argMap.at("ft").getArg();
	assert(argVal.has_value());
	cfg.fwdfilename = std::get<std::string>(argVal.value());

	argVal = argMap.at("path").getArg();
	assert(argVal.has_value());
	cfg.pathsfilename = std::get<std::string>(argVal.value());

	argVal = argMap.at("flag").getArg();
	assert(argVal.has_value());
    std::string flag = std::get<std::string>(argVal.value());
    assert(flag=="hop" || flag=="dist");
    cfg.hop = (flag=="hop");

	argVal = argMap.at("p").getArg();
	assert(argVal.has_value());
	cfg.pess = std::get<int>(argVal.value());

    Logging::LOGI(CONFIG_LOGM, "Populated argMap to Config");

    return cfg;
}