#include <iostream>
#include <vector>

#include "Buzzer.h"
#include "DeviceRegistry.h"
#include "FakeReadable.h"
#include "Interfaces.h"
#include "Relay.h"
#include "TemperatureSensor.h"

void printDevice(const Device& device) {
    std::cout << "printDevice: " << device.describe() << std::endl;
}

void switchAll(const std::vector<ISwitchable*>& targets, bool on) {
    for (ISwitchable* target : targets) {
        if (on) {
            target->turnOn();
        } else {
            target->turnOff();
        }
    }
}

void buzzerSwitch() {
    Relay fan{"fan", 12};
    Buzzer alarm{"alarm", 13, 10};
    switchAll({&fan, &alarm}, true);

    std::cout << "Relay: ";
    printDevice(fan);
    std::cout << "Buzzer: ";
    printDevice(alarm);
}

void deviceRegistry() {
    DeviceRegistry devices{};
    devices.add(std::make_unique<TemperatureSensor>("kitchen", 12, 21.0));
    devices.add(std::make_unique<Relay>("fan", 13));
    devices.add(std::make_unique<TemperatureSensor>("boiler", 14, 61.0));
    devices.add(std::make_unique<Buzzer>("alarm", 15, 10));

    devices.printAll();

    const int switched_of = devices.switchAllOff();
    std::cout << "кількість вимкнених: " << switched_of << std::endl;
    std::cout << "середня температура: " << devices.averageTemperature() << std::endl;
}

bool overheated(const IReadable& sensor, double limit) {
    return sensor.read() > limit;
}

void fakeSensor() {
    constexpr double kLimit = 60.0;

    FakeReadable fake_20{20.0};
    FakeReadable fake_85{85.0};

    std::cout << "fake 20: " << (overheated(fake_20, kLimit) ? " валідний" : " не валідний") << std::endl;
    std::cout << "fake 20 reads: " << fake_20.reads() << std::endl;
    std::cout << "fake 85: " << (overheated(fake_85, kLimit) ? " валідний" : " не валідний") << std::endl;
    std::cout << "fake 85 reads: " << fake_85.reads() << std::endl;

    TemperatureSensor boiler{"boiler", 12, 61.0};
    std::cout << "boiler 61: " << (overheated(boiler, kLimit) ? " валідний" : " не валідний") << std::endl;
}

int main() {
    // buzzerSwitch();

    // deviceRegistry();

    fakeSensor();
    return 0;
}