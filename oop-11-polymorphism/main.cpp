#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Device.h"
#include "Interfaces.h"
#include "Relay.h"
#include "TemperatureSensor.h"

namespace {

const char* yesNo(bool value) {
    return value ? "так" : "ні";
}

// 1. virtual: метод обирається за справжнім об'єктом, а не за типом змінної.
void printDevice(const Device& device) {
    std::cout << "printDevice: " << device.describe() << std::endl;
}

void virtualCall() {
    TemperatureSensor attic{"attic", 7, 18.0};
    Relay fan{"fan", 13};
    printDevice(attic);
    printDevice(fan);

    const Device& as_device = attic;
    std::cout << "as_device.describe();         " << as_device.describe() << std::endl;
    std::cout << "as_device.Device::describe(): " << as_device.Device::describe() << std::endl;
}

// 2. Віртуальний деструктор: знищення через вказівник на базу — повне.
void virtualDtor() {
    std::vector<std::unique_ptr<Device>> devices;
    devices.push_back(std::make_unique<TemperatureSensor>("boiler", 5, 40.0));
    devices.push_back(std::make_unique<Relay>("pump", 12));
    for (const std::unique_ptr<Device>& device : devices) {
        std::cout << "  " << device->describe() << std::endl;
    }
    std::cout << "-- devices.clear();" << std::endl;
    devices.clear();
    std::cout << "-- кінець функції" << std::endl;
}

// 3. Інтерфейси: функції знають лише «що вміє», а не «хто це».
double average(const std::vector<const IReadable*>& sources) {
    double sum = 0;
    for (const IReadable* source : sources) {
        sum += source->read();
    }

    return sources.empty() ? 0.0 : sum / static_cast<double>(sources.size());
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

void interfaces() {
    TemperatureSensor kitchen{"kitchen", 4, 21.0};
    TemperatureSensor street{"street", 6, -3.0};
    Relay fan{"fan", 13};
    Relay pump{"pump", 12};

    std::cout << "середня температура: " << average({&kitchen, &street}) << std::endl;
    switchAll({&fan, &pump}, true);
    std::cout << "fan увімкнено: " << yesNo(fan.isOn()) << std::endl;
    std::cout << "-- кінець функції" << std::endl;
}

// 4. dynamic_cast: «а чи вміє цей пристрій ще щось?»
void dynamicCast() {
    std::vector<std::unique_ptr<Device>> devices;
    devices.push_back(std::make_unique<TemperatureSensor>("kitchen", 4, 21.0));
    devices.push_back(std::make_unique<Relay>("fan", 13));

    for (const std::unique_ptr<Device>& device : devices) {
        std::cout << device->name() << ": ";
        if (auto* readable = dynamic_cast<IReadable*>(device.get())) {
            std::cout << "читається, " << readable->read() << std::endl;
        } else if (auto* switchable = dynamic_cast<ISwitchable*>(device.get())) {
            std::cout << "перемикається" << std::endl;
            switchable->turnOn();
        } else {
            std::cout << "нічого особливого" << std::endl;
        }
    }
    std::cout << "-- кінець функції" << std::endl;
}

// 5. Ціна virtual: прихований вказівник на таблицю віртуальних методів.
struct Plain {
    int value;
};

struct WithVirtual {
    virtual ~WithVirtual() = default;
    int value;
};

void sizes() {
    std::cout << "sizeof(Plain) = " << sizeof(Plain)
            << ", sizeof(WithVirtual) = " << sizeof(WithVirtual) << std::endl;
    std::cout << "sizeof(Device) = " << sizeof(Device) << std::endl;
    std::cout << "sizeof(IReadable) = " << sizeof(IReadable) << std::endl;
    std::cout << "sizeof(TemperatureSensor) = " << sizeof(TemperatureSensor) << std::endl;
}

// 6. Ромб: дві копії спільного предка — і virtual-успадкування.
struct Component {
    explicit Component(const std::string& source) : name(source) {
        std::cout << "      [Component] " << name << std::endl;
    }
    std::string name;
};

struct NetworkNode: Component {
    NetworkNode() : Component("network") {}
};

struct PowerNode: Component {
    PowerNode() : Component("power") {}
};

struct Gateway : NetworkNode, PowerNode {};

struct SharedNetworkNode: virtual Component {
    SharedNetworkNode() : Component("network") {}
};

struct SharedPowerNode : virtual Component {
    SharedPowerNode() : Component("power") {}
};

struct SharedGateway : SharedNetworkNode, SharedPowerNode {
    SharedGateway() : Component("gateway") {}
};

void diamond() {
    std::cout << "-- Gateway gateway;" << std::endl;
    Gateway gateway;
    std::cout << "NetworkNode::name = " << gateway.NetworkNode::name << ", PowerNode::name = " << gateway.PowerNode::name << std::endl;

    std::cout << "-- SharedGateway shared;" << std::endl;
    SharedGateway shared;
    std::cout << "shared.name = " << shared.name << std::endl;
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"virtual-call", virtualCall},
    {"virtual-dtor", virtualDtor},
    {"interfaces", interfaces},
    {"dynamic-cast", dynamicCast},
    {"sizes", sizes},
    {"diamond", diamond},
};

void printUsage() {
    std::cout << "Використання: oop-11 <експеримент>\n";
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
