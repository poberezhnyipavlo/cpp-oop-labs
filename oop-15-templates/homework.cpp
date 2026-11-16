#include <iostream>
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "Barometer.h"
#include "RingBuffer.h"
#include "Sampling.h"
#include "Sensor.h"

namespace {
double averageDynamic(const std::vector<std::unique_ptr<ISensor>>& sensors) {
    if (sensors.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (const auto& sensor : sensors) {
        sum += sensor->read();
    }

    return sum / static_cast<double>(sensors.size());
}

template <Readable S> double averageStatic(const std::vector<S>& sensors) {
    if (sensors.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (const auto& sensor : sensors) {
        sum += sensor.read();
    }
    return sum / static_cast<double>(sensors.size());
}
}
void ringBuffer() {
    RingBuffer<double, 4> readings;
    readings.push(21);
    readings.push(22);
    readings.push(23);
    readings.push(24);
    readings.push(25);

    for (std::size_t i = 0; i < readings.size(); i++) {
        std::cout << i << " елемент: " << readings.at(i) << "\n";
    }

    std::cout << "average: " << average(readings) << "\n";
    std::cout << "sizeof(readings): " << sizeof(readings) << "\n";

    RingBuffer<std::string, 2> names;
    names.push("a");
    names.push("b");
    names.push("c");

    for (std::size_t i = 0; i < names.size(); i++) {
        std::cout << i << " елемент: " << names.at(i) << "\n";
    }

    // std::cout << "average: " << average(names) << "\n";
    std::cout << "sizeof(names): " << sizeof(names) << "\n";

    // RingBuffer<int, 0> ints;
}

void averagesTwoWays() {
    std::unique_ptr<ISensor> v_thermometer = std::make_unique<VirtualThermometer>(20.0);
    std::unique_ptr<ISensor> v_thermometer_2 = std::make_unique<VirtualThermometer>(24.0);

    std::vector<std::unique_ptr<ISensor>> virtual_thermometers;
    virtual_thermometers.push_back(std::move(v_thermometer));
    virtual_thermometers.push_back(std::move(v_thermometer_2));

    const double average_dynamic = averageDynamic(virtual_thermometers);
    std::cout << "average dynamic: " << average_dynamic << "\n";

    std::vector<Thermometer> thermometers;
    thermometers.push_back(Thermometer{20.0});
    thermometers.push_back(Thermometer{24.0});

    const double average_static = averageStatic(thermometers);
    std::cout << "average static: " << average_static << "\n";
}

using AllSensors = std::variant<Thermometer, Hygrometer, Barometer>;

void barometerVariant() {
    std::vector<AllSensors> all_sensors;
    all_sensors.push_back(Thermometer{20.0});
    all_sensors.push_back(Hygrometer{24.0});
    all_sensors.push_back(Barometer{25.0});

    for (const auto& sensor : all_sensors) {
        std::visit([](const auto& concrete) { reportReadable(concrete); }, sensor);
    }

    std::cout << "sizeof(AllSensors): " << sizeof(AllSensors) << "\n";
}

int main() {
    // ringBuffer();

    // averagesTwoWays();

    barometerVariant();
    return 0;
}