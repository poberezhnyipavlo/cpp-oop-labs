#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Device.h"
#include "TemperatureSensor.h"

namespace {

// 1. Порядок: база → поля → тіло; знищення — навпаки.
void ctorOrder() {
    {
        std::cout << "-- TemperatureSensor kitchen{\"kitchen\", 4, 21.5};" << std::endl;
        TemperatureSensor kitchen{"kitchen", 4, 21.5};
        std::cout << "-- кінець блоку" << std::endl;
    }
}

// 2. Похідний — це база: методи бази доступні, protected — лише всередині.
void isA() {
    TemperatureSensor sensor{"boiler", 5, 40.0};
    std::cout << "name() з бази: " << sensor.name() << ", pin(): " << sensor.pin() << std::endl;
    sensor.update(42.5);
    std::cout << "sizeof(Device) = " << sizeof(Device)
              << ", sizeof(TemperatureSensor) = " << sizeof(TemperatureSensor) << std::endl;
}

// 3. Посилання і вказівник на базу: видно лише частину «Device».
void baseView() {
    TemperatureSensor sensor{"street", 6, -3.0};
    Device& as_device = sensor;
    const Device* as_pointer = &sensor;

    std::cout << "та сама адреса? " << (as_pointer == &sensor ? "так" : "ні") << std::endl;
    std::cout << "sensor.describe(): " << sensor.describe() << std::endl;
    std::cout << "as_device.describe(): " << as_device.describe() << std::endl;
    std::cout << "as_pointer->name(): " << as_pointer->name() << std::endl;
}

// 4. Зрізання: копія в об'єкт бази втрачає похідну частину.
void printByValue(Device device) {
    std::cout << "printByValue: " << device.describe() << std::endl;
}

void printByReference(const Device& device) {
    std::cout << "printByReference: " << device.describe() << std::endl;
}

void slicing() {
    TemperatureSensor sensor{"attic", 7, 18.0};

    std::cout << "-- Device copy = sensor;" << std::endl;
    Device copy = sensor;
    std::cout << "copy.describe(): " << copy.describe() << std::endl;

    std::cout << "-- printByReference(sensor);" << std::endl;
    printByReference(sensor);
    std::cout << "-- printByValue(sensor);" << std::endl;
    printByValue(sensor);

    std::cout << "-- std::vector<Device> devices; devices.push_back(sensor);" << std::endl;
    std::vector<Device> devices;
    devices.push_back(sensor);
    std::cout << "devices[0].describe(): " << devices[0].describe() << std::endl;
    std::cout << "-- кінець функції" << std::endl;
}
using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"ctor-order", ctorOrder},
    {"is-a", isA},
    {"base-view", baseView},
    {"slicing", slicing},
};

void printUsage() {
    std::cout << "Використання: oop-10 <експеримент>\n";
    for (const auto& [name, experiment] : kExperiments) {
        std::cout << "  " << name << '\n';
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2 || !kExperiments.contains(argv[1])) {
        printUsage();
        return 1;
    }

    std::cout << "main: старт\n";
    kExperiments.at(argv[1])();
    std::cout << "main: кінець\n";
    return 0;
}