#include "logger.h"
#include <fstream>
#include <iomanip>
#include <ctime>
#include <chrono>

Logger::Logger(const std::string& filePath) : filePath(filePath) {}

void Logger::log(const std::string& type, const std::string& details) const {
    std::ofstream file(filePath, std::ios::app);
    if (!file.is_open()) {
        return;
    }

    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    file << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S")
         << " - [" << type << "] " << details << "\n";
}