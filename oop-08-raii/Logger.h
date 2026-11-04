#pragma once

#include <string>

#include "File.h"

class Logger {
public:
    Logger(const std::string& main_path, const std::string& error_path);

    void info(const std::string& text);
    void error(const std::string& text);
    bool isOpen() const;

private:
    File main_;
    File errors_;
};
