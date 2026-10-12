#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

class Logger {
    private:
        std::string filePath;
public:
    explicit Logger(const std::string& filePath = "journal.log");
    void log(const std::string& type, const std::string& message) const;
};

#endif // LOGGER_H