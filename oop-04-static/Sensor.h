#pragma once

#include <string>

#include "Tracer.h"

class Sensor {
public:
    static constexpr  double kDefaultThreshold = 30.0;

    Sensor(const std::string& name, const Tracer& board);

    void print() const;

    static int created();

private:
    static int next_id_;

    const int id_;
    std::string name_;
    const Tracer& board_;
    double threshold_ = kDefaultThreshold;
};
