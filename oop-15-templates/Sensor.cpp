#include "Sensor.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

void reportDynamic(const ISensor& sensor) {
    std::cout << "      [virtual] " << sensor.name() << ": " << sensor.read() << "\n";
}

std::unique_ptr<ISensor> makeSensor(std::string_view kind) {
    if (kind == "thermometer") {
        return std::make_unique<VirtualThermometer>(21.5);
    }
    if (kind == "hygrometer") {
        return std::make_unique<VirtualHygrometer>(40.0);
    }
    throw std::invalid_argument("невідомий датчик: " + std::string{kind});
}
