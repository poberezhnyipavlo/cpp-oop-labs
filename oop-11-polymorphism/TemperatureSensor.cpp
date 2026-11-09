#include "TemperatureSensor.h"

#include <format>
#include <iostream>
#include <string>

#include "Device.h"

TemperatureSensor::TemperatureSensor(const std::string &name, int pin, double celsius)
    : Device(name, pin), celsius_(celsius) {
    std::cout << "      [TemperatureSensor] " << this->name() << ' ' << celsius_ << "C" << std::endl;
}

TemperatureSensor::~TemperatureSensor() {
    std::cout << "      [~TemperatureSensor] " << name() << std::endl;
}

std::string TemperatureSensor::describe() const {
    return std::format("датчик температури {}: {:.1f} C", name(), celsius_);
}

double TemperatureSensor::read() const {
    return celsius_;
}
