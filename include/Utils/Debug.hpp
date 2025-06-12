#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <string>

class Debug {
    public:
        static void Info(const std::string& message);
        static void Warning(const std::string& message);
        static void Error(const std::string& message);

    private:
        static std::string GetTimestamp();
        static const char* COLOR_RESET;
        static const char* COLOR_INFO;
        static const char* COLOR_WARNING;
        static const char* COLOR_ERROR;
};

#endif