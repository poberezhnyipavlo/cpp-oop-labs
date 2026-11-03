#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

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

void showByValue(FrameBuffer frame) {
    std::cout << "      showByValue: " << frame.name() << std::endl;
}

// 1. Конструктор копіювання чи присвоєння?
void whenWhich() {
    FrameBuffer a{"a", kSmallFrame};

    std::cout << "--- FrameBuffer b = a;" << std::endl;
    FrameBuffer b = a;

    std::cout << "--- FrameBuffer c{a};" << std::endl;
    FrameBuffer c{a};

    std::cout << "--- FrameBuffer d{\"d\", ...}; d = a;" << std::endl;
    FrameBuffer d{"d", kSmallFrame};
    d = a;

    std::cout << "--- showByValue(a);" << std::endl;
    showByValue(a);

    std::cout << "--- кінець функції" << std::endl;
}

// 2. Глибока копія: у кожного своя пам'ять.
void deepCopy() {
    FrameBuffer a{"a", kSmallFrame};
    a.fill(1);

    FrameBuffer b = a;
    b.setPixel(0, 9);

    std::cout << "a.pixel(0) = " << asNumber(a.pixel(0))
    << ", b.pixel(0) = " << asNumber(b.pixel(0)) << std::endl;
    std::cout << "спільна пам'ять? " << yesNo(a.sharesMemoryWith(b)) << std::endl;
}

// 3. Поверхнева копія: копіюється вказівник, а не дані.
struct ShallowBuffer {
    std::uint8_t* pixels;

    explicit ShallowBuffer(std::size_t size) : pixels(new std::uint8_t[size]{}) {}

    ~ShallowBuffer() {
        std::cout << "      [ShallowBuffer dtor] delete[] " << static_cast<const void*>(pixels) << std::endl;
        delete[] pixels;
    }
};

void shallowCopy() {
    ShallowBuffer a{kSmallFrame};
    ShallowBuffer b = a;
    b.pixels[0] = 9;

    std::cout << "a.pixels[0] = " << asNumber(a.pixels[0]) << std::endl;
    std::cout << "спільна пам'ять? " << yesNo(a.pixels == b.pixels) << std::endl;
    std::cout << "--- кінець функції" << std::endl;
}

// 4. Самоприсвоєння.
void selfAssign() {
    FrameBuffer a{"a", kSmallFrame};
    a.fill(7);

    FrameBuffer& alias = a;
    a = alias;

    std::cout << "a.pixel[0] = " << asNumber(a.pixel(0)) << std::endl;
}

// 5. Вектор копіює, коли росте.
void vectorCopies() {
    FrameBuffer a{"a", kSmallFrame};
    std::vector<FrameBuffer> frames;

    std::cout << "-- push_back #1" << std::endl;
    frames.push_back(a);
    std::cout << "-- push_back #2" << std::endl;
    frames.push_back(a);
    std::cout << "-- push_back #3(тимчасовий)" << std::endl;
    frames.push_back(FrameBuffer{"tmp", kSmallFrame});

    std::cout << "-- кінець функції" << std::endl;
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"when-which", whenWhich},
    {"deep-copy", deepCopy},
    {"shallow-copy", shallowCopy},
    {"self-assign", selfAssign},
    {"vector-copies", vectorCopies},
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