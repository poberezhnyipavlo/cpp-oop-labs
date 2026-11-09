#pragma once

#include <string>

#include "Device.h"
#include "Interfaces.h"

class Relay final: public Device, public ISwitchable {
public:
    Relay(const std::string& name, int pin);
    ~Relay() override;

    void turnOn() override;
    void turnOff() override;

    bool isOn() const override;

    std::string describe() const override;
private:
    bool on_ = false;
};
