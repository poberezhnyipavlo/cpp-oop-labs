#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "Tracer.h"

namespace {

// 1. Лямбда — це об'єкт класу з operator(). Ось такий клас вручну:
class Multiplier {
public:
    explicit Multiplier(int factor) : factor_(factor) {}
    int operator()(int value) const {
        return value * factor_;
    };

private:
    int factor_;
};

void closureObject() {
    const Multiplier by_hand{3};
    int factor = 3;
    const auto by_lambda = [factor](int value) { return value * factor; };
    const auto no_capture = [](int value) { return value * 2; };

    std::cout << "by_hand(7) = " << by_hand(7) << ", by_lambda(7) = " << by_lambda(7) << std::endl;
    std::cout << "sizeof(Multiplier) = " << sizeof(by_hand)
              << ", sizeof(by_lambda) = " << sizeof(by_lambda)
              << ", sizeof(no_capture) = " << sizeof(no_capture) << std::endl;
}

// 2. Захоплення за значенням (копія на момент створення) і за посиланням.
void captureModes() {
    int threshold = 20;
    const auto by_value = [threshold](int value) { return value > threshold; };
    const auto by_reference = [&threshold](int value) { return value > threshold; };

    std::cout << "threshold = 20: by_value(25) = " << by_value(25)
              << ", by_reference(25) = " << by_reference(25) << std::endl;
    threshold = 30;
    std::cout << "threshold = 30: by_value(25) = " << by_value(25)
              << ", by_reference(25) = " << by_reference(25) << std::endl;

    std::cout << "-- Tracer sensor{\"sensor\"}" << std::endl;
    const Tracer sensor{"sensor"};
    std::cout << "-- auto copy_capture = [sensor] {...};" << std::endl;
    const auto copy_capture = [sensor] { return sensor.name(); };
    std::cout << "-- auto ref_capture = [&sensor] {...};" << std::endl;
    const auto ref_capture = [&sensor] { return sensor.name(); };
    std::cout << "copy_capture() = " << copy_capture() << ", ref_capture() = " << ref_capture()
              << std::endl;
    std::cout << "-- кінець функції" << std::endl;
}

// 3. Власний стан лямбди: ініціалізоване захоплення і mutable.
void mutableState() {
    auto next_id = [count = 0]() mutable { return ++count; };
    std::cout << "next_id: " << next_id() << ' ' << next_id() << ' ' << next_id() << std::endl;

    auto copy = next_id;
    std::cout << "copy: " << copy() << " next_id: " << next_id() << std::endl;
}

// 4. std::function — «будь-що, що можна викликати з такою сигнатурою».
void stdFunction() {
    std::vector<std::function<void(double)>> handlers;
    handlers.push_back([](double celsius) {
        std::cout << "      log: " << celsius << " C" << std::endl;
    });

    int alarms = 0;
    handlers.push_back([&alarms](double celsius) {
        if (celsius > 60.0) {
            ++alarms;
        }
    });

    const Multiplier doubler{2};
    handlers.push_back([doubler](double celsius) {
        std::cout << "      doubled: " << doubler(static_cast<int>(celsius)) << std::endl;
    });

    for (const double reading : {21.5, 65.0, 70.0}) {
        for (const std::function<void(double)>& handler : handlers) {
            handler(reading);
        }
    }

    std::cout << "alarms: " << alarms << std::endl;
    std::cout << "sizeof(std::function<void(double)>) = " << sizeof(std::function<void(double)>)
              << std::endl;
}

// 5. Захоплення за посиланням, що пережило змінну.
void danglingCapture() {
    std::function<std::string()> describe;
    {
        const std::string name = "kitchen-temperature-sensor-01";
        describe = [&name] { return "датчик " + name; };
        std::cout << "у блоці: " << describe() << std::endl;
    }
    std::cout << "після блоку: " << describe() << std::endl;
}

// 6. std::jthread копіює аргументи; std::ref — передати посилання.
void report(const Tracer& tracer) {
    std::cout << "      потік: " << tracer.name() << std::endl;
}

void threadArguments() {
    const Tracer tracer{"probe"};
    std::cout << "-- std::jthread worker{report, tracer};" << std::endl;
    {
        const std::jthread worker{report, tracer};
    }

    std::cout << "-- std::jthread worker{report, std::cref(tracer)};" << std::endl;
    {
        const std::jthread worker{report, std::cref(tracer)};
    }

    std::cout << "-- std::jthread worker{[&tracer] { report(tracer); }" << std::endl;
    {
        const std::jthread worker{[&tracer] { report(tracer); }};
    }

    std::cout << "-- кінець функції" << std::endl;
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"closure-object", closureObject},
    {"capture-modes", captureModes},
    {"mutable-state", mutableState},
    {"std-function", stdFunction},
    {"dangling-capture", danglingCapture},
    {"thread-arguments", threadArguments},
};

void printUsage() {
    std::cout << "Використання: oop-14 <експеримент>\n";
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

    std::cout << std::boolalpha;
    std::cout << "main: старт\n";
    kExperiments.at(argv[1])();
    std::cout << "main: кінець, живих Tracer: " << Tracer::alive() << '\n';
    return 0;
}