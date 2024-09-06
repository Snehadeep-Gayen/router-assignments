#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

namespace CommandLine{
    namespace Logging{

    inline std::string getCurrentTimeWithMicroseconds() 
    {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        auto now_us = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_c), "%H:%M:%S") << "." << std::setfill('0') << std::setw(6) << now_us.count();
        return ss.str();
    }

    inline void LOGX(std::string level, std::string module, std::string message) 
    {
        std::cout << "[" << getCurrentTimeWithMicroseconds() << "]"; 
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
