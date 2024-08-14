#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include "Argument.hpp"
#include "ArgumentInfo.hpp"
#include <vector>

namespace CommandLine {

    #define CLP_LOGMODULE "CommandLineParser"

    class ArgumentMapFactory {
    
    public:

        std::unordered_map<std::string, Argument> parse(
            int argc,
            char* argv[],
            std::string filename="cli.json");

    private:

        std::unordered_map<std::string, Argument> parse(
            int argc,
            char* argv[],
            std::vector<ArgumentInfo> argInfos);

    };
    
}