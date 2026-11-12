#include "TemperatureMonitor.h"

#include <iostream>

void TemperatureMonitor::onReading(double celsius) const {
    if (celsius > limit_) {
        std::cout << "ALARM <" << celsius << "> > <" << limit_ << ">" << std::endl;
    }
}

TemperatureMonitor::TemperatureMonitor(EventBus& bus, double limit)
    : bus_(bus), limit_(limit), id_(bus_.subscribe([this](double celsius) { onReading(celsius); })) {
}

TemperatureMonitor::~TemperatureMonitor() {
    bus_.unsubscribe(id_);
}