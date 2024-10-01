#pragma once

#include <string>
#include <optional>
#include "ArgumentType.hpp"
#include "Logging.hpp"

namespace CommandLine {

#define ARGUMENT_LOGMODULE "Argument_Class"

    class Argument {
    public:
        Argument(std::string flag, std::string description, std::optional<ArgumentType> arg);
        Argument(std::string flag);
        Argument() = default;
        virtual ~Argument() = default;

        std::string getFlag() const;
        std::string getDescription() const;
        std::optional<ArgumentType> getArg() const;
    
    private:
        std::string flag;
        std::string description;
        std::optional<ArgumentType> arg;
    };
    
}