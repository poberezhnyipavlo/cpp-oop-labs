#include "TemperatureSensor.h"

#include <format>
#include <iostream>
#include <string>

#include "Device.h"

TemperatureSensor::TemperatureSensor(const std::string &name, int pin, double celsius)
    : Device(name, pin), probe_(name + "-probe"), celsius_(celsius) {
    std::cout << "      [TemperatureSensor] " << this->name() << ' ' << celsius_ << "C" << std::endl;
}

TemperatureSensor::~TemperatureSensor() {
    std::cout << "      [~TemperatureSensor] " << name() << std::endl;
}

double TemperatureSensor::celsius() const {
    return celsius_;
}

void TemperatureSensor::update(double celsius) {
    celsius_ = celsius;
    log(std::format("датчик температури {}: {:.1f} C", name(), celsius_));
}

std::string TemperatureSensor::describe() const {
    return std::format("датчик температури {}: {:.1f} C", name(), celsius_);
}
