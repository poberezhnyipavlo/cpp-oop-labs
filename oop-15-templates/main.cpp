#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "Sampling.h"
#include "Sensor.h"

namespace {

// 1. Той самий «інтерфейс» двома способами.
void sameInterface() {
    const VirtualThermometer v_thermo{21.5};
    const VirtualHygrometer v_hygro{40.0};
    reportDynamic(v_thermo);
    reportDynamic(v_hygro);

    const Thermometer thermo{21.5};
    const Hygrometer hygro{40.0};
    reportStatic(thermo);
    reportStatic(hygro);

    std::cout << "sizeof(VirtualThermometer) = " << sizeof(VirtualThermometer) << ", sizeof(Thermometer) = " << sizeof(Thermometer) << "\n";
}

// 2. Інстанціювання: шаблон — не функція, а рецепт; функцію компілятор робить для кожного T.
void instantiation() {
    countCalls<Thermometer>();
    countCalls<Thermometer>();
    countCalls<Hygrometer>();
    countCalls<double>();
}

// 3. Тип відомий лише під час виконання — тут шаблон не допоможе.
void runtimeChoice() {
    const std::vector<std::string> config{"thermometer", "hygrometer", "thermometer"};
    std::vector<std::unique_ptr<ISensor>> sensors;
    for (const std::string& kind : config) {
        sensors.push_back(makeSensor(kind));
    }
    for (const std::unique_ptr<ISensor>& sensor : sensors) {
        reportDynamic(*sensor);
    }
}

// 4. Закритий набір типів без спільної бази: std::variant + std::visit.
using AnySensor = std::variant<Thermometer, Hygrometer>;

void variantSensors() {
    const std::vector<AnySensor> sensors{Thermometer{21.5}, Hygrometer{40.0}};
    for (const AnySensor& sensor : sensors) {
        std::visit([](const auto& concrete) { reportStatic(concrete);}, sensor);
    }
    std::cout << "sizeof(AnySensor) = " << sizeof(AnySensor) << ", index of sensors[1] = " << sensors[1].index() << "\n";
}

// 5. Концепти: вимоги до типу, перевірені компілятором.
void concepts() {
    std::cout << "Readable<Thermometer> = " << Readable<Thermometer> << "\n";
    std::cout << "Readable<VirtualThermometer> = " << Readable<VirtualThermometer> << "\n";
    std::cout << "Readable<Relay> = " << Readable<Relay> << "\n";
    std::cout << "Readable<double> = " << Readable<double> << "\n";
    reportReadable(Thermometer{19.0});
    reportReadable(VirtualThermometer{55.0});
}

// 6. Виведення аргументів шаблону.
void deduction() {
    std::cout << "clampReading(75, 0, 50) = " << clampReading(75, 0, 50) << "\n";
    std::cout << "clampReading(21.7, 0.0, 50.0) = " << clampReading(21.7, 0.0, 50.0) << "\n";
    std::cout << "clampReading<double>(75, 0, 50.5) = " << clampReading<double>(75, 0, 50.5) << "\n";
}
using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"same-interface", sameInterface},
    {"instantiation", instantiation},
    {"runtime-choice", runtimeChoice},
    {"variant", variantSensors},
    {"concepts", concepts},
    {"deduction", deduction},
};

void printUsage() {
    std::cout << "Використання: oop-15 <експеримент>\n";
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
    kExperiments.at(argv[1])();
    return 0;
}
