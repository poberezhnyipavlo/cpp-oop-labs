#pragma once

#include <compare>
#include <ostream>

// Тип-значення: температура в градусах Цельсія. Оператори — як у вбудованих чисел.
class Temperature {
public:
    explicit  Temperature(double celsius);

    double celsius() const;

    Temperature operator+(double delta) const;
    Temperature& operator+=(double delta);

    bool operator==(const Temperature& other) const = default;
    auto operator<=>(const Temperature& other) const = default;

    friend std::ostream& operator<<(std::ostream& out, const Temperature& temperature);

private:
    double celsius_;
};
