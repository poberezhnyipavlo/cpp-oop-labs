#pragma once

#include "Device.h"
#include "Interfaces.h"

class TemperatureSensor : public Device, public IReadable {
public:
    TemperatureSensor(const std::string& name, int pin, double celsius);
    ~TemperatureSensor() override;

    std::string describe() const override;
    double read() const override;

private:
    double celsius_;
};
