#include "Sensor.h"

#include <iostream>
#include <string>

#include "Tracer.h"

int Sensor::next_id_ = 1;

Sensor::Sensor(const std::string& name, const Tracer& board)
    : id_(next_id_++), name_(name), board_(board) {
    std::cout << "      [Sensor #" << id_ << "] " << name_ << " on '" << board_.name() << "'" << std::endl;
}

void Sensor::print() const {
    std::cout << "Sensor #" << id_ << " '" << name_ << "' on '" << board_.name() << "', threshold "
              << threshold_ << '\n';
}

int Sensor::created() {
    return next_id_ - 1;
}
