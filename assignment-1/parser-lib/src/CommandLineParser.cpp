#include "CommandLineParser.hpp"
#include <fstream>
#include <cassert>

namespace CommandLine {

    using namespace Logging;

    std::unordered_map<std::string, Argument> ArgumentMapFactory::parse(
        int argc,
        char* argv[],
        std::string filename) 
    {
        LOGI(CLP_LOGMODULE, "Parsing arguments from file: "+filename);
        // open the file
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file"+filename);
        }
        LOGI(CLP_LOGMODULE, "Successfully opened file: "+filename);

        // read the file
        char ch;
        file >> ch;
        assert(ch == '[');

        std::vector<ArgumentInfo> argInfos;
        while (1) 
        {
            // check for ]
            file >> ch;
            if (ch == ']')
                break;
            if (ch == ',')
                file >> ch;
            assert(ch == '{');
            std::string json_elem = std::string() + ch;
            while (1) 
            {
                file >> ch;
                json_elem = json_elem + ch;
                if (ch == '}')
                    break;
            }
            LOGI(CLP_LOGMODULE, "Read json_elem: "+json_elem);
            argInfos.push_back(*ArgumentInfo::ParseJson(json_elem));
        }
        LOGI(CLP_LOGMODULE, "Successfully read argument info from file");
        
        return parse(argc, argv, argInfos);
    }

    std::unordered_map<std::string, Argument> ArgumentMapFactory::parse(
        int argc,
        char* argv[],
        std::vector<ArgumentInfo> argInfos) 
    {
        std::unordered_map<std::string, Argument> argMap;
        for(int i=1; i<argc; i++) 
        {
            std::string arg = argv[i];
            if (arg[0] == '-')
            {
                for (auto& argInfo : argInfos) 
                {
                    if (argInfo.flagMatches(arg)) 
                    {
                        Argument argument = argInfo.getArgument(argv[++i]);
                        argMap[argument.getFlag()] = argument;
                        LOGI(CLP_LOGMODULE, "Parsed argument: "+argument.getFlag());
                        break;
                    }
                }
            }
        }
        // Log the parsed arguments
        LOGI(CLP_LOGMODULE, "Final parsed arguments: ");
        for (auto& [flag, _] : argMap) 
        {
            LOGI(CLP_LOGMODULE, flag);
        }
        return argMap;
    }

}