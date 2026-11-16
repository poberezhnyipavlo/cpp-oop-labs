#pragma once

#include <string>

class Barometer {
public:
    explicit Barometer(double hpa) : hpa_(hpa) {}
    std::string name() const {
        return "barometer";
    }
    double read() const {
        return hpa_;
    }
private:
    double hpa_;
};
