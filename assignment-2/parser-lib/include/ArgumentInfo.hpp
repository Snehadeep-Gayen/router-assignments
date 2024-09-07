#pragma once

#include <memory>
#include <string>
#include "ArgumentType.hpp"
#include "Argument.hpp"
#include "Logging.hpp"

namespace CommandLine {

    #define ARGUMENT_INFO_LOGMODULE "ArgumentInfo"

    class ArgumentInfo{
    public:
        static std::unique_ptr<ArgumentInfo> ParseJson(std::string json_elem);

        bool flagMatches(std::string flag) const;
        Argument getArgument(std::string value) const;
        ArgumentType convertToType(std::string value) const;

    private:
        enum _ArgType {
            INT,
            DOUBLE,
            STRING,
            BOOL
        };

        ArgumentInfo() : flag(""), argType(_ArgType::STRING), defaultArg(""), optional(false), description("No description provided") {}

        std::string flag;
        _ArgType argType;
        ArgumentType defaultArg;
        bool optional;
        std::string description;
    };

}