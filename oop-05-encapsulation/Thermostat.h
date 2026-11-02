#pragma once

class Thermostat {
public:
    static constexpr double kMinCelsius = 5;
    static constexpr double kMaxCelsius = 30;
    static constexpr double kDefaultCelsius = 21.0;

    bool setTarget(double celsius);
    double target() const;
    bool isHeating() const;
    void update(double current);
private:
    double target_ = kDefaultCelsius;
    bool heating_= false;
};

void printStatus(const Thermostat& thermostat);