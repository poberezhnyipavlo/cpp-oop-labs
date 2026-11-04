#include "Logger.h"

#include <iostream>
#include <string>

#include "File.h"

Logger::Logger(const std::string& main_path, const std::string& error_path)
    : main_(main_path), errors_(error_path) {
    std::cout << "      [Logger ready]" << std::endl;
}

void Logger::info(const std::string& text) {
    main_.write(text + '\n');
}

void Logger::error(const std::string& text) {
    errors_.write(text + '\n');
}

bool Logger::isOpen() const {
    return main_.isOpen() && errors_.isOpen();
}
