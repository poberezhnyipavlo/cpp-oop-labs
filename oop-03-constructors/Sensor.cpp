#include "Sensor.h"

#include <iostream>
#include <string>

#include "Tracer.h"

Sensor::Sensor(int id, const std::string& name, const Tracer& board, double threshold)
    : id_(id), name_(name), board_(board), threshold_(threshold) {
    std::cout << "      [Sensor 4 args] " << name_ << std::endl;
}

Sensor::Sensor(int id, const std::string& name, const Tracer& board)
    : id_(id), name_(name), board_(board) {
    std::cout << "      [Sensor 3 args] " << name_ << std::endl;
}

Sensor::Sensor(int id, const Tracer& board) : Sensor(id, "sensor" + std::to_string(id), board) {
    std::cout << "      [Sensor 2 args] " << name_ << std::endl;
}

void Sensor::print() const {
    std::cout << "Sensor #" << id_ << " '" << name_ << "' on '" << board_.name() << "', threshold "
              << threshold_ << std::endl;
}