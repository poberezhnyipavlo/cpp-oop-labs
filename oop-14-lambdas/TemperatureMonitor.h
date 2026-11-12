#pragma once
#include "EventBus.h"

class TemperatureMonitor {
public:
    TemperatureMonitor(EventBus& bus, double limit);
    ~TemperatureMonitor();

    TemperatureMonitor(const TemperatureMonitor&) = delete;
    TemperatureMonitor& operator=(const TemperatureMonitor&) = delete;

private:
    EventBus& bus_;
    double limit_;
    int id_;

    void onReading(double celsius) const;
};
