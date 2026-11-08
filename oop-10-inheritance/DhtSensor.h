#pragma once
#include "TemperatureSensor.h"

class DhtSensor : public TemperatureSensor {
public:
    DhtSensor(const std::string& name, int pin, double celsius, double humidity);
    ~DhtSensor();
    using TemperatureSensor::TemperatureSensor;

    double humidity() const;

private:
    double humidity_ = 0.0;
};
