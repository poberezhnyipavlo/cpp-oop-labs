#include "Temperature.h"

#include <format>
#include <ostream>

Temperature::Temperature(double celsius) : celsius_(celsius) {}

double Temperature::celsius() const {
    return celsius_;
}

Temperature Temperature::operator+(double delta) const {
    return Temperature{celsius_ + delta};
}

Temperature& Temperature::operator+=(double delta) {
    celsius_ += delta;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Temperature& temperature) {
    return out << std::format("{:.1f} C", temperature.celsius_);
}
