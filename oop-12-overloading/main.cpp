#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "SampleBuffer.h"
#include "Temperature.h"

namespace {

// 1. Перевантаження: одна назва — кілька функцій, вибір за типами аргументів.
void show(int value) {
    std::cout << "  show(int) " << value << std::endl;
}

void show(double value) {
    std::cout << "  show(double) " << value << std::endl;
}

void show(bool value) {
    std::cout << "  show(bool) " << (value ? "true" : "value") << std::endl;
}

void show(const std::string& value) {
    std::cout << "  show(std::string) " << value << std::endl;
}

void pickOverload() {
    std::cout << "-- show(42)" << std::endl;
    show(42);
    std::cout << "-- show(4.2)" << std::endl;
    show(4.2);
    std::cout << "-- show(4.2f)" << std::endl;
    show(4.2f);
    std::cout << "-- show('A')" << std::endl;
    show('A');
    std::cout << "-- show(std::string{\"dht\"}" << std::endl;
    show(std::string{"dht"});
    std::cout << "-- show(\"dht\")" << std::endl;
    show("dht");
}

// 2. Оператори свого типу.
void operators() {
    Temperature kitchen{21.5};
    const Temperature warmer = kitchen + 2.0;
    kitchen += 0.5;

    std::cout << "kitchen = " << kitchen << ", warmer = " << warmer << std::endl;
    std::cout << "kitchen == warmer? " << (kitchen == warmer ? "так" : "ні") << std::endl;
    std::cout << "kitchen < warmer? " << (kitchen < warmer ? "так" :  "ні") << std::endl;

    std::vector<Temperature> readings{Temperature{30.0}, Temperature{-5.0}, Temperature{18.5}};
    std::sort(readings.begin(), readings.end());
    std::cout << "відсортовано:";
    for (const Temperature& reading : readings) {
        std::cout << ' ' << reading;
    }
    std::cout << std::endl;
}

// 3. operator[]: перевантаження за const.
void printFirst(const SampleBuffer& buffer) {
    const double first = buffer[0];
    std::cout << "printFirst: " << first << std::endl;
}

void constOverload() {
    SampleBuffer buffer{3};
    std::cout << "--buffer[0] = 21.5;" << std::endl;
    buffer[0] = 21.5;
    std::cout << "-- printFirst(buffer);" << std::endl;
    printFirst(buffer);
}

// 4. Приховування: метод нащадка ховає ВСІ однойменні методи бази.
struct Base {
    void set(int value) {
        std::cout << "      Base::set(int) " << value << std::endl;
    }
    void set(const std::string& value) {
        std::cout << "      Base::set(std::string) " << value << std::endl;
    }
};

struct Hiding : Base {
    void set(double value) {
        std::cout << "      Hiding::set(double) " << value << std::endl;
    }
};

struct Unhiding : Base {
    using Base::set;
    void set(double value) {
        std::cout << "      Unhiding::set(double) " << value << std::endl;
    }
};

void hiding() {
    Hiding hiding;
    std::cout << "--hiding.set(5)" << std::endl;
    hiding.set(5);
    std::cout << "-- hiding.Base::set(5)" << std::endl;
    hiding.Base::set(5);

    Unhiding unhiding;
    std::cout << "-- unhiding.set(5)" << std::endl;
    unhiding.set(5);
    std::cout << "-- unhiding.set(5.5)" << std::endl;
    unhiding.set(5.5);
    std::cout << "-- unhiding.set(std::string{\"x\"})" << std::endl;
    unhiding.set(std::string{"x"});
}

// 5. Аргументи за замовчуванням і virtual: значення береться за типом змінної.
struct Sensor {
    virtual ~Sensor() = default;
    virtual void sample(int count = 1) const {
        std::cout << "      Sensor::sample(" << count << ")" << std::endl;
    }
};

struct FastSensor : Sensor {
    void sample(int count = 10) const override {
        std::cout << "      FastSensor::sample(" << count << ")" << std::endl;
    }
};

void defaultArgs() {
    const FastSensor fast;
    const Sensor& as_base = fast;
    std::cout << "-- fast.sample()" << std::endl;
    fast.sample();
    std::cout << "-- as_base.sample()" << std::endl;
    as_base.sample();
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"pick-overload", pickOverload},
    {"operators", operators},
    {"const-overload", constOverload},
    {"hiding", hiding},
    {"default-args", defaultArgs},
};

void printUsage() {
    std::cout << "Використання: oop-12 <експеримент>\n";
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
