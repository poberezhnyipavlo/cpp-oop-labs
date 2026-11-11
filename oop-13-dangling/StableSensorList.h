#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Sensor.h"

class StableSensorList {
public:
    Sensor& add(const std::string& name);
private:
    std::vector<std::unique_ptr<Sensor>> sensors_;
};
