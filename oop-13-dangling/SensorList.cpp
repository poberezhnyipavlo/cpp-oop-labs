#include "SensorList.h"

#include <string>

#include "Sensor.h"

Sensor& SensorList::add(const std::string& name) {
    sensors_.emplace_back(name);
    return sensors_.back();
}