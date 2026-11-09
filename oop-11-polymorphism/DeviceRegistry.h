#pragma once

#include <vector>

#include "Device.h"

class DeviceRegistry {
public:
    void add(std::unique_ptr<Device> device);
    double averageTemperature() const;
    int switchAllOff();
    void printAll() const;
private:
    std::vector<std::unique_ptr<Device>> devices_;
};
