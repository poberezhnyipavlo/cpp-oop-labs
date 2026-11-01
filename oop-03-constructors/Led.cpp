#include "Led.h"

#include <algorithm>
#include <iostream>

namespace {

constexpr int kMinBrightness = 0;
constexpr int kMaxBrightness = 255;

int clampBrightness(int brightness) {
    return std::clamp(brightness, kMinBrightness, kMaxBrightness);
}

}  // namespace

Led::Led(int pin, const std::string& name, int brightness)
    : pin_(pin), name_(name), brightness_(clampBrightness(brightness)) {
    std::cout << "      [ctor] " << name_ << std::endl;
}

Led::Led(int pin, const std::string& name) : pin_(pin), name_(name) {
    std::cout << "      [Led 2 parameters] " << name_ << std::endl;
}

Led::Led(int pin) : Led(pin, "pin" + std::to_string(pin)) {
    std::cout << "      [Led 1 parameter] " << name_ << std::endl;
}

Led::~Led() {
    std::cout << "      [dtor] " << name_ << std::endl;
}

void Led::print() const {
    std::cout << "Led #" << pin_ << ", name: " << name_ << ", brightness: " << brightness_
              << std::endl;
}
