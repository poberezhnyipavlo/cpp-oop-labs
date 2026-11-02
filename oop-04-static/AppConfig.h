#pragma once

#include <string>

struct AppConfig {
    std::string device;
    int port = 0;
};

AppConfig loadConfig();

const AppConfig& appConfig();


