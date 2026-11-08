#include <iostream>
#include <ostream>

#include "DhtSensor.h"
#include "Relay.h"
#include "TemperatureSensor.h"

void relayChain() {
    Relay pump{"pump", 12};
    pump.turnOn();
    std::cout << "pump.describe(): " << pump.describe() << std::endl;

    const Device& device = pump;
    std::cout << "device.describe(): " << device.describe() << std::endl;
}

void printAll(const std::vector<Device>& devices) {
    for (const Device& device : devices) {
        std::cout << device.describe() << std::endl;
    }
}

void printAllPointers(const std::vector<const Device*>& devices) {
    for (const Device* device : devices) {
        std::cout << device->describe() << std::endl;
    }
}

void slicingTrap() {
    TemperatureSensor attic{"attic", 7, 18.0};
    Relay fan{"fan", 13};

    {
        std::vector<Device> copies;
        copies.push_back(attic);
        copies.push_back(fan);

        printAll(copies);
    }

    {
        std::vector<const Device*> pointers{&attic, &fan};
        printAllPointers(pointers);
    }
}

void threeLevels() {
    {
        DhtSensor dht{"dht22", 15, 23.0, 55.0};
        std::cout << "dht.humidity(): " << dht.humidity() << ", dht.celsius(): " << dht.celsius() << std::endl;
    }

    {
        DhtSensor inherited{"dht11", 16, 20.0};
        std::cout << "dht.humidity(): " << inherited.humidity() << ", dht.celsius(): " << inherited.celsius() << std::endl;
    }

}

int main() {
    // relayChain();

    // slicingTrap();

    threeLevels();
    return 0;
}