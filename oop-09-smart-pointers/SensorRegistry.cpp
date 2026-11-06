#include "SensorRegistry.h"

#include <memory>
#include <string>
#include <cstddef>
#include <utility>

#include "Tracer.h"

void SensorRegistry::add(std::unique_ptr<Tracer> sensor) {
    sensors_.push_back(std::move(sensor));
}

Tracer* SensorRegistry::find(const std::string& name) const {
    for (const std::unique_ptr<Tracer>& sensor : sensors_) {
        if (sensor->name() == name) {
            return sensor.get();
        }
    }

    return nullptr;
}

std::unique_ptr<Tracer> SensorRegistry::take(const std::string &name) {
    for (auto it = sensors_.begin(); it != sensors_.end(); ++it) {
        if ((*it)->name() == name) {
            std::unique_ptr<Tracer> sensor = std::move(*it);
            sensors_.erase(it);

            return sensor;
        }
    }

    return nullptr;
}

std::size_t SensorRegistry::size() const {
    return sensors_.size();
}
