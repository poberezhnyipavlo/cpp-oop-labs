#include "Relay.h"

#include <iostream>
#include <ostream>
#include <string>

Relay::Relay(const std::string& name, int pin) : Device(name, pin) {
    std::cout << "      [Relay] <" << this->name() << ">" << std::endl;
}

Relay::~Relay() {
    std::cout << "      [~Relay] <" << name() << ">" << std::endl;
}

void Relay::turnOn() {
    on_ = true;
    log("увімкнено");
}

void Relay::turnOff() {
    on_ = false;
    log("вимкнено");
}

bool Relay::isOn() const {
    return on_;
}

std::string Relay::describe() const {
    return "реле <" + name() + "> " + (on_ ? "увімкнено" : "вимкнено");
}
