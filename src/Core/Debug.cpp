#include "Core/Debug.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

const char* Debug::COLOR_RESET = "\033[0m";
const char* Debug::COLOR_INFO = "\033[36m"; // Cyan
const char* Debug::COLOR_WARNING = "\033[33m"; // Yellow
const char* Debug::COLOR_ERROR = "\033[31m"; // Red

std::string Debug::GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void Debug::Info(const std::string& message) {
    std::cout 
        << COLOR_INFO
        << "[" << GetTimestamp() << "] [INFO] " << message 
        << COLOR_RESET << std::endl;
}

void Debug::Warning(const std::string& message) {
    std::cout 
        << COLOR_WARNING
        << "[" << GetTimestamp() << "] [WARNING] " << message 
        << COLOR_RESET << std::endl;
}

void Debug::Error(const std::string& message) {
    std::cerr 
        << COLOR_ERROR
        << "[" << GetTimestamp() << "] [ERROR] " << message 
        << COLOR_RESET << std::endl;
}
