#include "DeviceRegistry.h"

#include <iostream>
#include <ostream>

#include "Interfaces.h"

void DeviceRegistry::add(std::unique_ptr<Device> device) {
    devices_.push_back(std::move(device));
}

double DeviceRegistry::averageTemperature() const {
    double sum = 0;
    int count = 0;

    for (const std::unique_ptr<Device>& device : devices_) {
        if (const auto* item = dynamic_cast<IReadable*>(device.get())) {
            sum += item->read();
            ++count;
        }
    }

    return count == 0 ? 0.0 : sum / count;
}

void DeviceRegistry::printAll() const {
    for (const std::unique_ptr<Device>& device : devices_) {
        std::cout << "DeviceRegistry::printAll(): " << device->describe() << std::endl;
    }
}

int DeviceRegistry::switchAllOff() {
    int count = 0;

    for (const std::unique_ptr<Device>& device : devices_) {
        if (auto* item = dynamic_cast<ISwitchable*>(device.get())) {
            item->turnOff();
            ++count;
        }
    }

    return count;
}


