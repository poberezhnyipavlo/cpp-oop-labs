#include "Thermostat.h"

#include <iostream>

bool Thermostat::setTarget(double celsius) {
    if (celsius < kMinCelsius || celsius > kMaxCelsius) {
        std::cout << "      [add] відхилено " << celsius << std::endl;
        return false;
    }
    target_ = celsius;
    return true;
}

double Thermostat::target() const {
    return target_;
}

bool Thermostat::isHeating() const {
    return heating_;
}

void Thermostat::update(double current) {
    if (current > target_ + 0.5) {
        heating_ = false;
    }
    if (current < target_ - 0.5) {
        heating_ = true;
    }
}

void printStatus(const Thermostat& thermostat) {
    std::cout << "target: " << thermostat.target() << ", heating: " << thermostat.isHeating() << std::endl;
}
