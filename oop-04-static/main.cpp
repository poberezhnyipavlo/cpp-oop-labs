#include <iostream>
#include <map>
#include <string>

#include "Boards.h"
#include "Sensor.h"
#include "Tracer.h"

namespace {
// Глобальний об'єкт у main.cpp, що залежить від глобального об'єкта з Boards.cpp.
const Sensor kEarlySensor{"early", g_main_board};

// 1. Глобальні об'єкти: до main і після main.
void globals() {
    std::cout << "kEarlySensor: ";
    kEarlySensor.print();
}

// 2. static-поле — одне на весь клас.
void staticMember() {
    std::cout << "створення до експерименту: " << Sensor::created() << std::endl;
    const Sensor a{"temp", g_main_board};
    const Sensor b{"humidity", g_main_board};
    const Sensor c{"light", g_main_board};
    std::cout << "створено після: " << Sensor::created() << std::endl;
    std::cout << "sizeof(Sensor) = " << sizeof(Sensor) << std::endl;
}

// 3. static-метод і static constexpr — без об'єкта.
void staticMethod() {
    std::cout << "Sensor::created() = " << Sensor::created() << std::endl;
    std::cout << "Sensor::kDefaultThreshold = " << Sensor::kDefaultThreshold << std::endl;
    const Sensor s{"pressure", g_main_board};
    std::cout << "s.created() = " << s.created() << std::endl;
}

// 4. Локальна static у функції.
int nextTicket() {
    static int ticket = 0;
    ++ticket;
    return ticket;
}

void localCounter() {
    std::cout << "ticket " << nextTicket() << std::endl;
    std::cout << "ticket " << nextTicket() << std::endl;
    std::cout << "ticket " << nextTicket() << std::endl;
}

// 5. Локальна static-об'єкт: створення при першому виклику.
void lazyBoard() {
    std::cout << "до першого виклику sparedBoard()" << std::endl;
    const Sensor a{"spare-1", spareBoard()};
    const Sensor b{"spare-2", spareBoard()};
    std::cout << "кінець lazyBoard()" << std::endl;
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"globals", globals},
    {"static-member", staticMember},
    {"static-method", staticMethod},
    {"local-counter", localCounter},
    {"lazy-board", lazyBoard},
};

void printUsage() {
    std::cout << "Використання: oop-04 <експеримент>\n";
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
    std::cout << "main: кінець, живих Tracer: " << Tracer::alive() << '\n';
    return 0;
}