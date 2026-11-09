#include "Buzzer.h"

#include <iostream>

Buzzer::Buzzer(const std::string& name, int pin, int frequency)
    : Device(name, pin), frequency_(frequency) {
    std::cout << "      [Buzzer] <" << this->name() << ">" << std::endl;
}

Buzzer::~Buzzer() {
    std::cout << "      [~Buzzer] <" << name() << ">" << std::endl;
}

std::string Buzzer::describe() const {
    return "пищалка <" + name() + "> <frequency> " + std::to_string(frequency_) + "Гц" +
           (on_ ? " пищить" : " тиша");
}

void Buzzer::turnOn() {
    on_ = true;
    log("пищить");
}

void Buzzer::turnOff() {
    on_ = false;
    log("тиша");
}

bool Buzzer::isOn() const {
    return on_;
}
