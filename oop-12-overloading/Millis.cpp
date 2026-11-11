#include "Millis.h"

#include <ostream>

Millis Millis::operator+(const Millis& other) const {
    return Millis{value_ + other.value_};
}

Millis Millis::operator-(const Millis& other) const {
    return Millis{value_ - other.value_};
}

Millis& Millis::operator+=(const Millis& other) {
    value_ += other.value_;
    return *this;
}

long long Millis::count() const {
    return value_;
}

std::ostream& operator<<(std::ostream& out, const Millis& millis) {
    return out << "<" << millis.value_ << "> ms";
}