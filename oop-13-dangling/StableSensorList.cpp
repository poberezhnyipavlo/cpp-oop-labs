#include "StableSensorList.h"

#include <string>
#include <vector>
#include <memory>

#include "Sensor.h"

Sensor& StableSensorList::add(const std::string& name) {
    sensors_.push_back(std::make_unique<Sensor>(name));

    return *sensors_.back();
}
