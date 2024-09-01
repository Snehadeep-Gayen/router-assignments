#include <iostream>
#include "CommandLineParser.hpp"
#include "ArgumentType.hpp"
#include <cassert>

/*
[
    {
      "flag": "top",
      "type": "string"
    },
    {
      "flag": "conn",
      "type": "string"
    },
    {
      "flag": "rt",
      "type": "string"
    },
    {
      "flag": "ft",
      "type": "string"
    },
    {
      "flag": "path",
      "type": "string"
    },
    {
      "flag": "flag",
      "type": "string"
    },
    {
      "flag": "p",
      "type": "int"
    }
]
*/

int main()
{
    // dummy command line arguments
    int argc = 15;
    const char* argv[] = {"./test_main", "-top", "top_val", "-conn", "conn_val", "-rt", "rt_val", "-ft", "ft_val", "-path", "path_val", "-flag", "flag_val", "-p", "10"};

    const auto& argMap = CommandLine::ArgumentMapFactory().parse(argc, (char**) argv, std::string("cli.json"));

    std::cout << "------------------" << std::endl;
    std::cout << "Parsed arguments: " << std::endl;
    std::cout << "------------------" << std::endl;

    std::optional<CommandLine::ArgumentType> argVal = argMap.at("top").getArg();
    assert(argVal.has_value());
    std::cout << "top" << ":" << std::get<std::string>(argVal.value()) << std::endl;

    argVal = argMap.at("conn").getArg();
    assert(argVal.has_value());
    std::cout << "conn" << ":" << std::get<std::string>(argVal.value()) << std::endl;

    argVal = argMap.at("rt").getArg();
    assert(argVal.has_value());
    std::cout << "rt" << ":" << std::get<std::string>(argVal.value()) << std::endl;

    argVal = argMap.at("ft").getArg();
    assert(argVal.has_value());
    std::cout << "ft" << ":" << std::get<std::string>(argVal.value()) << std::endl;

    argVal = argMap.at("path").getArg();
    assert(argVal.has_value());
    std::cout << "path" << ":" << std::get<std::string>(argVal.value()) << std::endl;

    argVal = argMap.at("flag").getArg();
    assert(argVal.has_value());
    std::cout << "flag" << ":" << std::get<std::string>(argVal.value()) << std::endl;

    argVal = argMap.at("p").getArg();
    assert(argVal.has_value());
    std::cout << "p" << ":" << std::get<int>(argVal.value()) << std::endl;

    std::cout << "------------------" << std::endl;

    return 0;
}
