#pragma once

#include <string>

#include "Tracer.h"

class Sensor {
public:
    Sensor(int id, const std::string& name, const Tracer& board, double threshold);
    Sensor(int id, const std::string& name, const Tracer& board);
    Sensor(int id, const Tracer& board);

    void print() const;

private:
    const int id_;
    std::string name_;
    const Tracer& board_;
    double threshold_ = 30.0;
};
