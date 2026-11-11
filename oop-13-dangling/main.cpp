#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "Sensor.h"

namespace {

// Довгі імена — щоб рядок жив у купі (коротші влазять у сам об'єкт std::string).
std::string makeName() {
    return "kitchen-temperature-sensor-01";
}

Sensor makeSensor() {
    Sensor sensor{"boiler-temperature-sensor-02"};
    sensor.add(40.0);
    sensor.add(42.5);
    return sensor;
}

// 1. const& до тимчасового: життя ПРОДОВЖЕНО до кінця блоку.
void extendTemp() {
    const std::string& kept = makeName();
    std::cout << "kept: " << kept << std::endl;
}

// 2. Посилання на поле тимчасового об'єкта: продовження НЕ працює.
void memberOfTemp() {
    const std::string& name = Sensor{"temporary-sensor-for-demo-03"}.name();
    std::cout << "-- після рядка з тимчасовим Sensor" << std::endl;
    std::cout << "name: " << name << std::endl;
}

// 3. string_view з тимчасового рядка — баг із SM-2.
struct Message {
    std::string_view sensor;
    double value;
};

std::string readLine() {
    return "kitchen-temperature-sensor-01=21.5";
}

Message parse(std::string_view line) {
    const std::size_t equals = line.find('=');
    return Message{line.substr(0, equals), std::stod(std::string{line.substr(equals + 1)})};
}

void viewOfTemp() {
    const Message message = parse(readLine());
    std::cout << "value: " << message.value << std::endl;
    std::cout << "sensor: " << message.sensor << std::endl;
}

// 4. Посилання на елемент вектора після росту.
void vectorGrowth() {
    std::vector<double> samples{21.5};
    const double& first = samples[0];
    std::cout << "first до росту: " << first << ", capacity = " << samples.capacity() << std::endl;
    for (int i = 0; i < 10; i++) {
        samples.push_back(static_cast<double>(i));
    }
    std::cout << "capacity після = " << samples.capacity() << std::endl;
    std::cout << "first після росту: " << first << std::endl;
}

// 5. range-for по полю тимчасового об'єкта (C++20).
void rangeForTemp() {
    for (const double sample : makeSensor().samples()) {
        std::cout << "sample: " << sample << std::endl;
    }
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"extend-temp", extendTemp},
    {"member-of-temp", memberOfTemp},
    {"view-of-temp", viewOfTemp},
    {"vector-growth", vectorGrowth},
    {"range-for-temp", rangeForTemp},
};

void printUsage() {
    std::cout << "Використання: oop-13 <експеримент>\n";
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