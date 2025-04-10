#pragma once

#include <string>
#include <variant>

namespace CommandLine {

    using ArgumentType = std::variant<int, double, std::string, bool>;

}