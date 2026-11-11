#pragma once

#include "Sensor.h"

class SensorList {
public:
    Sensor& add(const std::string& name);
private:
    std::vector<Sensor> sensors_;
};
