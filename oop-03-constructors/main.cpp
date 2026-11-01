#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Sensor.h"
#include "Tracer.h"

namespace {
void printVector(const std::string& label, const std::vector<int>& values) {
    std::cout << label << ": size " << values.size() << ", [";
    for (std::size_t i = 0; i < values.size(); ++i) {
        std::cout << (i == 0 ? "" : ", ") << values[i];
    }
    std::cout << "]" << std::endl;
}

// 1. Конструктор за замовчуванням, що делегує іншому.
void defaultCtor() {
    Tracer t;
    std::cout << "t = " << t.name() << std::endl;
}

// 2. Три конструктори Sensor: делегування й ініціалізатор поля.
void sensors() {
    Tracer board{"esp32"};

    std::cout << "-- 4 аргументи" << std::endl;
    Sensor a{1, "temp", board, 45.5};
    a.print();

    std::cout << "-- 3 аргументи" << std::endl;
    Sensor b{2, "humidity", board};
    b.print();

    std::cout << "-- 2 аргументи" << std::endl;
    Sensor c{3, board};
    c.print();
}
// 3. Ініціалізація у списку проти присвоєння в тілі.
struct InitInList {
    Tracer tag;

    explicit InitInList(const std::string& name) : tag(name) {}
};

struct AssignOnBody {
    Tracer tag;

    explicit AssignOnBody(const std::string& name) {
        tag = Tracer{name};
    }
};

void initVsAssign() {
    std::cout << "-- InitInList" << std::endl;
    InitInList a{"list"};

    std::cout << "-- AssignOnBody" << std::endl;
    AssignOnBody b{"body"};

    std::cout << "-- кінець функції" << std::endl;
}

// 4. explicit: перетворення лише явно.
void report(const Tracer& t) {
    std::cout << "      report:" << t.name() << std::endl;
}

void explicitCtor() {
    const std::string name{"from-string"};
    report(Tracer{name});
    std::cout << "після report" << std::endl;
}

// 5. {} проти ().
void braces() {
    printVector("std::vector<int> a(3)", std::vector<int>(3));
    printVector("std::vector<int> b{3}", std::vector<int>{3});
    printVector("std::vector<int> c(3, 7)", std::vector<int>(3, 7));
    printVector("std::vector<int> d{3, 7}", std::vector<int>{3, 7});

    const double measured = 3.7;
    const int truncated(measured);
    std::cout << "int truncated = " << truncated << std::endl;
}

// 6. = default і value-ініціалізація.
struct Point {
    int x;
    int y;

    Point() = default;
};

void defaultKeyword() {
    Point p{};

    std::cout << "Point p{}: x = " << p.x << ", y = " << p.y << std::endl;
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"default-ctor", defaultCtor},
    {"sensors", sensors},
    {"init-vs-assign", initVsAssign},
    {"explicit", explicitCtor},
    {"braces", braces},
    {"default-keyword", defaultKeyword},
};

void printUsage() {
    std::cout << "Використання: oop-03 <експеримент>\n";
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