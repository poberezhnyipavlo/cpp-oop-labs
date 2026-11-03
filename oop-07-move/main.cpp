#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility>

#include "FrameBuffer.h"

namespace {

constexpr std::size_t kSmallFrame = 4;

const char* yesNo(bool value) {
    return value ? "так" : "ні";
}

// Для друку uint8_t як числа, а не символу.
int asNumber(std::uint8_t value) {
    return static_cast<int>(value);
}

void printState(const std::string& label, const FrameBuffer& frame) {
    std::cout << label << ": name = " << frame.name() << ", size = " << frame.size()
              << ", порожній? " << yesNo(frame.isEmpty()) << std::endl;
}

// 1. Конструктор перенесення: забрати буфер, а не копіювати.
void moveCtor() {
    FrameBuffer a{"a", kSmallFrame};
    a.fill(7);

    FrameBuffer b = std::move(a);

    printState("a", a);
    printState("b", b);
    std::cout << "b.pixel(0) = " << asNumber(b.pixel(0)) << std::endl;
}

// 2. std::move — лише каст; іменоване && — вже lvalue.
void moveIsCast() {
    FrameBuffer a{"a", kSmallFrame};

    FrameBuffer&& ref = std::move(a);
    printState("a після std::move", a);

    std::cout << "-- FrameBuffer b = ref;" << std::endl;
    FrameBuffer b = ref;

    std::cout << "-- FrameBuffer c = std::move(ref);" << std::endl;
    FrameBuffer c = std::move(ref);
    printState("a", a);
}

// 3. Присвоєння перенесенням: старий буфер звільняється, новий забирається.
void moveAssign() {
    FrameBuffer a{"a", kSmallFrame};
    FrameBuffer d{"d", kSmallFrame};

    d = std::move(a);

    printState("a", a);
    printState("d", d);
}

// 4. Вектор переносить, коли росте (бо move — noexcept).
void vectorMoves() {
    FrameBuffer a{"a", kSmallFrame};
    std::vector<FrameBuffer> frames;

    std::cout << "-- push_back #1 (копія a)" << std::endl;
    frames.push_back(a);
    std::cout << "-- push_back #2 (копія a)" << std::endl;
    frames.push_back(a);
    std::cout << "-- push_back #3 (тимчасовий)" << std::endl;
    frames.push_back(FrameBuffer{"temp", kSmallFrame});
    std::cout << "-- push_back #4 (std::move(a))" << std::endl;
    frames.push_back(std::move(a));

    std::cout << "-- кінець функції" << std::endl;
}

// 5. Повернення за значенням: ні копії, ні переносу.
FrameBuffer makeFrame(const std::string& name) {
    FrameBuffer frame{name, kSmallFrame};
    frame.fill(1);
    return frame;
}

void returnValue() {
    FrameBuffer frame = makeFrame("made");
    printState("frame", frame);
}

// 6. Перенесений об'єкт можна знищити або присвоїти йому нове значення.
void reuseMovedFrom() {
    FrameBuffer a{"a", kSmallFrame};
    FrameBuffer b = std::move(a);

    a = FrameBuffer{"fresh", kSmallFrame};
    printState("a", a);
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"move-ctor", moveCtor},
    {"move-is-cast", moveIsCast},
    {"move-assign", moveAssign},
    {"vector-moves", vectorMoves},
    {"return-value", returnValue},
    {"reuse-moved-from", reuseMovedFrom},
};

void printUsage() {
    std::cout << "Використання: oop-06 <експеримент>\n";
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