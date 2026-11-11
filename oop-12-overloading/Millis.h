#pragma once

#include <compare>
#include <ostream>

class Millis {
public:
    static constexpr long long kMillisPerSecond = 1000;
    static constexpr long long kMillisPerMinute = 60 * kMillisPerSecond;
    static constexpr long long kMillisPerHour = 60 * kMillisPerMinute;

    explicit constexpr Millis(long long value) : value_(value) {}

    long long count() const;

    Millis operator+(const Millis& other) const;
    Millis operator-(const Millis& other) const;

    Millis& operator+=(const Millis& other);

    auto operator<=>(const Millis&) const = default;

    friend std::ostream& operator<<(std::ostream& out, const Millis& millis);
private:
    long long value_;
};

constexpr Millis operator""_ms(unsigned long long value) {
    return Millis(static_cast<long long>(value));
}

constexpr Millis operator""_s(unsigned long long value) {
    return Millis(static_cast<long long>(value) * Millis::kMillisPerSecond);
}

constexpr Millis operator""_min(unsigned long long value) {
    return Millis(static_cast<long long>(value) * Millis::kMillisPerMinute);
}

constexpr Millis operator""_h(unsigned long long value) {
    return Millis(static_cast<long long>(value) * Millis::kMillisPerHour);
}
