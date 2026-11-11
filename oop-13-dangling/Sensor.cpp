#include "Sensor.h"

#include <iostream>
#include <string>
#include <vector>

Sensor::Sensor(const std::string& name) : name_(name) {
    std::cout << "      [Sensor] " << name_ << std::endl;
}

Sensor::~Sensor() {
    std::cout << "      [~Sensor] " << name_ << std::endl;
}

const std::string& Sensor::name() const {
    return name_;
}

const std::vector<double>& Sensor::samples() const {
    return samples_;
}

void Sensor::add(double sample) {
    samples_.push_back(sample);
}
