#pragma once

#include "Device.h"
#include "Interfaces.h"
#include <string>

class Buzzer final : public Device, public ISwitchable {
public:
    Buzzer(const std::string& name, int pin, int frequency);
    ~Buzzer() override;

    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;

    std::string describe() const override;
private:
    int frequency_;
    bool on_ = false;
};
