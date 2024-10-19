#include "ArgumentInfo.hpp"
#include <sstream>
#include <utility>
#include <cassert>
#include <iostream>

namespace CommandLine {

using namespace Logging;

    std::pair<std::string, std::string> GetKeyValue(std::stringstream& ss)
    {
        char ch;
        std::string key;
        ss >> ch;
        while (ch != ':')
        {
            key += ch;
            ss >> ch;
        }
        assert(key.back()=='\"');
        key = key.substr(0, key.size()-1);

        // get value
        std::string value;
        ss >> ch;
        assert(ch == '\"');
        do
        {
            value += ch;
            ss >> ch;
        } while (ch != '\"');
        value += ch;
        assert(value[0]=='\"' && value.back()=='\"');
        value = value.substr(1, value.size()-2);

        return {key, value};
    }

    std::unique_ptr<ArgumentInfo> ArgumentInfo::ParseJson(std::string json_elem) 
    {
        // json_elem is a string in the format:
        // {
        //     "flag": "-f",
        //     "type": "int",
        //     "optional": false,
        //     "default": 0,
        //     "description": "This is a description"
        // }
        // Note that: type should be before default

        LOGD(ARGUMENT_INFO_LOGMODULE, "Parsing json_elem: "+json_elem);

        ArgumentInfo argInfo;

        std::stringstream ss(json_elem);
        char ch;
        ss >> ch;
        if (ch != '{')
            throw std::invalid_argument("Invalid json_elem");

        bool argTypeSet = false;
        while(1)
        {
            // check for }
            ss >> ch;
            if (ch == '}')
                break;
            if (ch == ',')
                ss >> ch;

            auto [key, value] = GetKeyValue(ss);
            LOGD(ARGUMENT_INFO_LOGMODULE, "Key: "+key+" Value: "+value);

            if (key == "flag")
                argInfo.flag = value;
            else if (key == "type")
            {
                if (value == "int")
                    argInfo.argType = _ArgType::INT;
                else if (value == "double")
                    argInfo.argType = _ArgType::DOUBLE;
                else if (value == "string")
                    argInfo.argType = _ArgType::STRING;
                else if (value == "bool")
                    argInfo.argType = _ArgType::BOOL;
                else
                    throw std::invalid_argument("Invalid type");
                argTypeSet = true;
            }
            else if (key == "optional")
            {
                if (value == "true")
                    argInfo.optional = true;
                else if (value == "false")
                    argInfo.optional = false;
                else
                    throw std::invalid_argument("Invalid optional");
            }
            else if (key == "description")
                argInfo.description = value;
            else if (key == "default")
            {
                if(!argTypeSet)
                    throw std::invalid_argument("Type not set before default value");
                argInfo.defaultArg = argInfo.convertToType(value);
            }
        }
        
        return std::make_unique<ArgumentInfo>(argInfo);
    }

    bool ArgumentInfo::flagMatches(std::string flag) const {
        LOGI(ARGUMENT_INFO_LOGMODULE, "Checking if flag " + this->flag + " matches: "+flag);
        return std::string("-")+this->flag == flag;
    }

    Argument ArgumentInfo::getArgument(std::string value) const {
        Argument arg(flag, description, convertToType(value));
        return arg;
    }

    ArgumentType ArgumentInfo::convertToType(std::string value) const {
        switch (argType) {
            case _ArgType::INT:
                return std::stoi(value);
            case _ArgType::DOUBLE:
                return std::stod(value);
            case _ArgType::STRING:
                return value;
            case _ArgType::BOOL:
                if (value == "true" || value == "1")
                    return true;
                else if (value == "false" || value == "0")
                    return false;
                else
                    throw std::invalid_argument("Invalid boolean value");
        }
    }

}