#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "Car.h"
#include "Tracer.h"

namespace {
// 1. Стек: об'єкт живе до кінця свого блоку.
void scope() {
    std::cout << "вхід у функцію\n";
    Tracer a{"a"};
    {
        std::cout << "вхід у внутрішній блок\n";
        Tracer b{"b"};
        Tracer c{"c"};
        std::cout << "вихід з внутрішнього блоку\n";
    }
    Tracer d{"d"};
    std::cout << "вихід з функції\n";
}

// 2. Поля живуть усередині об'єкта-власника.
void members() {
    Car car;
    std::cout << "sizeof(Car) = " << sizeof(car) << ", sizeof(Tracer) =" << sizeof(Tracer) << '\n';
}

// 3. Купа: об'єкт переживає блок, у якому створений.
Tracer* makeOnHeap() {
    auto* p = new Tracer{"heap"};
    std::cout << "makeOnHeap: повертає адресу\n";
    return p;
}

void heap() {
    Tracer* p = makeOnHeap();
    std::cout << "після makeOnHeap: " << p->name() << '\n';
    delete p;
    std::cout << "після delete\n";
}

// 4. Забутий delete.
void leak() {
    auto* p = new Tracer{"leaked"};
    std::cout << "забули delete для " << p->name() << '\n';
}

// 5. delete є, але виняток пролітає повз нього.
void mayThrow() {
    throw std::runtime_error{"шось пішло не так"};
}

void exceptionLeak() {
    auto* p = new Tracer{"raw"};
    mayThrow();
    delete p;
}

// 6. Те саме з unique_ptr.
void exceptionSafe() {
    auto p = std::make_unique<Tracer>("owned");
    mayThrow();
}

// 7. Локальна static.
Tracer& registry() {
    static Tracer instance{"registry"};
    return instance;
}

void localStatic() {
    std::cout << "перший виклик\n";
    registry();
    std::cout << "другий виклик\n";
    registry();
}

struct Reading {
    int value = 0;
};

void doubleDelete() {
    auto* p = new Reading{21};
    delete p;
    delete p;
}

Tracer* addressOf(Tracer& t) {
    return &t;
}

void deleteStack() {
    Tracer local{"local"};
    Tracer* p = addressOf(local);
    delete p;
}

void useAfterDelete() {
    auto* p = new Reading{42};
    delete p;
    std::cout << "value після delete: " << p->value << '\n';
}

using Expirement = void(*)();

const std::map<std::string, Expirement> kExperiments{
    {"scope", scope},
    {"members", members},
    {"heap", heap},
    {"leak", leak},
    {"exception-leak", exceptionLeak},
    {"exception-safe", exceptionSafe},
    {"static", localStatic},
    {"double-delete", doubleDelete},
    {"delete-stack", deleteStack},
    {"use-after-delete", useAfterDelete},
};

void printUsage() {
    std::cout << "Використання: oop-01 <експеримент>\n";
    for (const auto& [name, experiment] : kExperiments) {
        std::cout << " " << name << '\n';
    }
}

void runGuarded(Expirement expirement) {
    try {
        expirement();
    } catch (const std::exception& e) {
        std::cout << "main: спіймано виняток: " << e.what() << '\n';
    }
}
}

int main(int argc, char* argv[]) {
    if (argc != 2 || !kExperiments.contains(argv[1])) {
        printUsage();
        return 1;
    }

    std::cout << "main: старт\n";
    runGuarded(kExperiments.at(argv[1]));
    std::cout << "main: кінець, живих Tracer: " << Tracer::alive() << '\n';
    
    return 0;
}