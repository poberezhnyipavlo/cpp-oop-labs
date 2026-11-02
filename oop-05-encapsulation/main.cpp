#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "ReadingBuffer.h"

namespace {

ReadingBuffer makeBuffer() {
    ReadingBuffer buffer;
    buffer.add(20.0);
    buffer.add(22.0);
    buffer.add(24.0);
    return buffer;
}

// 1. Через const& — лише const-методи.
void printSummary(const ReadingBuffer& buffer) {
    const double average = buffer.average();
    std::cout << "size = " << buffer.size() << ", average = " << average << std::endl;
}

void constView() {
    ReadingBuffer buffer = makeBuffer();
    printSummary(buffer);
}

// 2. mutable: кеш у const-методі.
void cache() {
    ReadingBuffer buffer = makeBuffer();
    printSummary(buffer);
    printSummary(buffer);
    buffer.add(30.0);
    printSummary(buffer);
}

// 3. Інваріант: клас не пускає неправильні дані.
struct RawReadings {
    std::vector<double> values;
};

void invariant() {
    ReadingBuffer buffer;
    buffer.add(21.5);
    buffer.add(-300.0);
    buffer.add(1000.0);
    std::cout << "ReadingBuffer: size = " << buffer.size() << std::endl;

    RawReadings raw;
    raw.values.push_back(21.5);
    raw.values.push_back(-300.0);
    raw.values.push_back(1000.0);
    std::cout << "RawReadings: size = " << raw.values.size() << std::endl;
}

// 4. friend бачить private.
void friendFunction() {
    const ReadingBuffer buffer = makeBuffer();
    dumpRaw(buffer);
    buffer.average();
    dumpRaw(buffer);
}

// 5. Перевантаження за const і посилання, що «протікає» назовні.
void leakyReference() {
    ReadingBuffer buffer = makeBuffer();
    const ReadingBuffer& view = buffer;

    const double first = view.at(0);
    std::cout << "view.at(0) = " << first << std::endl;
    printSummary(buffer);

    buffer.set(0, 100.0);
    printSummary(buffer);
    dumpRaw(buffer);
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"const-view", constView},
    {"cache", cache},
    {"invariant", invariant},
    {"friend", friendFunction},
    {"leaky-reference", leakyReference},
};

void printUsage() {
    std::cout << "Використання: oop-05 <експеримент>\n";
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
