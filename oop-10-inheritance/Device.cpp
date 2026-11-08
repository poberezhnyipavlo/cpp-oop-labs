#include "Device.h"

#include <iostream>
#include <string>

Device::Device(const std::string &name, int pin) : name_(name), pin_(pin) {
    std::cout << "      [Device] " << name_ << " pin=" << pin_ <<std::endl;
}

Device::~Device() {
    std::cout << "      [~Device] " << name_ << std::endl;
}

const std::string& Device::name() const {
    return name_;
}

int Device::pin() const {
    return pin_;
}

std::string Device::describe() const {
    return "пристрій " + name_ + " на піні " + std::to_string(pin_);
}

void Device::log(const std::string &message) const {
    std::cout << "      {" << name_ << "} " << message << std::endl;
}
