#include "DhtSensor.h"

#include <iostream>
#include <ostream>
#include <string>

DhtSensor::DhtSensor(const std::string& name, int pin, double celsius, double humidity)
    : TemperatureSensor(name, pin, celsius), humidity_(humidity) {
    std::cout << "      [DhtSensor] <" << this->name() << "> <humidity>" << humidity_ << "%" << std::endl;
}

DhtSensor::~DhtSensor() {
    std::cout << "      [~DhtSensor] <" << name() << ">" << std::endl;
}

double DhtSensor::humidity() const {
    return humidity_;
}
