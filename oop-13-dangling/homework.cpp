#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>

#include "SensorList.h"
#include "StableSensorList.h"
#include "Sensor.h"

std::string readLine() {
    return "kitchen-temperature-sensor-01=21.5";
}

struct OwnedMessage {
    std::string sensor;
    double value;
};

OwnedMessage parseOwned(std::string_view line) {
    const std::size_t equals = line.find('=');
    return OwnedMessage{
        .sensor = std::string{line.substr(0, equals)},
        .value = std::stod(std::string{line.substr(equals + 1)})
    };
}

void ownedMessage() {
    const OwnedMessage message = parseOwned(readLine());
    std::cout << "value: " << message.value << std::endl;
    std::cout << "sensor: " << message.sensor << std::endl;
}

void stableReferences() {
    // StableSensorList list{};
    //
    // Sensor& first = list.add("kitchen-temperature-sensor-01");
    // list.add("b");
    // list.add("c");
    // first.add(1.0);
    //
    // std::cout << "first: " << first.name() << std::endl;

    SensorList list{};

    Sensor& first = list.add("kitchen-temperature-sensor-01");
    list.add("b");
    list.add("c");
    first.add(1.0);

    std::cout << "first: " << first.name() << std::endl;
}

std::string_view trimmed(std::string_view text) {
     while (!text.empty() && (text.front() == ' ' || text.back() == ' ')) {
        if (text.front() == ' ') {
            text.remove_prefix(1);
        }
        if (text.back() == ' ') {
            text.remove_suffix(1);
        }
     }

     return text;
}

std::string padded() {
    return "   kitchen-temperature-sensor-01   ";
}

void fullExpression() {
    std::cout << "[" << trimmed(padded()) << "]" << std::endl;
    const std::string_view kept = trimmed(padded());
    std::cout << "kept: " << kept << std::endl;
}

int main() {
    // ownedMessage();

    // stableReferences();

    fullExpression();
    return 0;
}