#include "Core/Debug.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

void Debug::Log(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    std::cout 
        << "["
        << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S")
        << "] " << message 
        << std::endl;
}