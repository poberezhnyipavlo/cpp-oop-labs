#pragma once

#include <string>

#include "Device.h"

class Relay : public Device {
public:
    Relay(const std::string& name, int pin);
    ~Relay();

    void turnOn();
    void turnOff();

    bool isOn() const;

    std::string describe() const;
private:
    bool on_ = false;
};
