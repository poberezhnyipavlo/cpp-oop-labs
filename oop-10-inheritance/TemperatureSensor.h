#pragma once

#include "Device.h"
#include "Tracer.h"

// Похідний клас: датчик температури — це пристрій (is-a) + своє значення.
class TemperatureSensor : public Device {
public:
    TemperatureSensor(const std::string& name, int pin, double celsius);
    ~TemperatureSensor();

    double celsius() const;
    void update(double celsius);
    std::string describe() const;

private:
    Tracer probe_;
    double celsius_;
};
