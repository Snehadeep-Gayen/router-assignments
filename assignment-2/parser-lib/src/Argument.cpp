#include "Argument.hpp"

namespace CommandLine {

    Argument::Argument(std::string flag, std::string description, std::optional<ArgumentType> arg)
        : flag(flag), description(description), arg(arg) {}

    Argument::Argument(std::string flag)
        : flag(flag), description("No description provided"), arg(std::nullopt) {}

    std::string Argument::getFlag() const {
        return flag;
    }

    std::string Argument::getDescription() const {
        return description;
    }

    std::optional<ArgumentType> Argument::getArg() const {
        return arg;
    }

}