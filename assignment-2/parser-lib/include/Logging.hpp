#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

namespace CommandLine{
    namespace Logging{

    inline void LOGX(std::string level, std::string module, std::string message) 
    {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::cout << "[" << std::put_time(std::localtime(&now_c), "%H:%M:%S") << "]"; 
        std::cout << " [" << level << "] ";
        std::cout << " [" << module << "] " << message << std::endl;
    }

    inline void LOGI(std::string module, std::string message) 
    {
        LOGX("INFO", module, message);
    }

    inline void LOGE(std::string module, std::string message) 
    {
        LOGX("ERROR", module, message);
    }

    inline void LOGW(std::string module, std::string message) 
    {
        LOGX("WARNING", module, message);
    }

    inline void LOGD(std::string module, std::string message) 
    {
        LOGX("DEBUG", module, message);
    }

    }   
}
