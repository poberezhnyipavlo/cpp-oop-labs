#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Millis.h"

void millis() {
    Millis blink = 500_ms;
    Millis total = blink + 200_ms;
    total += 1000_ms;

    std::cout << "total: " << total << std::endl;
    std::cout << "blink: " << blink << std::endl;
    std::cout << "total > blink " << (total > blink ? "yes" : "no") << std::endl;

    std::cout << 1_s << '\n';           // <1000> ms
    std::cout << 1_min << '\n';         // <60000> ms
    std::cout << 1_h << '\n';           // <3600000> ms
    std::cout << 1_s + 500_ms << '\n';  // <1500> ms
}

void setLevel(int level) {
    std::cout << "setLevel<" << level << ">" << std::endl;
}

void setLevel(double level) = delete;
void setLevel(bool level) = delete;

void deletedOverloads() {
    setLevel(3);
    setLevel('A');

    // setLevel(3.7);
    // setLevel(true);
}

struct Reading {
    std::string name;
    double value;
    auto operator<=>(const Reading&) const = default;
};

void printReadings(const std::vector<Reading>& readings) {
    for (const Reading& reading : readings) {
        std::cout << reading.name << ": " << reading.value << std::endl;
    }
}

void sortReadings() {
    std::vector<Reading> readings;
    readings.push_back(Reading{"street", -3.0});
    readings.push_back(Reading{"kitchen", 21.0});
    readings.push_back(Reading{"boiler", 61.0});
    readings.push_back(Reading{"kitchen", 19.5});

    std::sort(readings.begin(), readings.end());

    std::cout << "стандартне сортування: " << std::endl;
    printReadings(readings);

    std::sort(readings.begin(), readings.end(), [](const Reading& a, const Reading& b) {
        return a.value > b.value;
    });

    std::cout << "сортування через колбек: " << std::endl;
    printReadings(readings);

    std::ranges::sort(readings, {}, &Reading::value);

    std::cout << "сортування через C++20 проєкцію: " << std::endl;
    printReadings(readings);
}

int main() {
    // millis();

    // deletedOverloads();

    sortReadings();

    return 0;
}