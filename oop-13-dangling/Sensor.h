#pragma once

#include <string>
#include <vector>

// Датчик з ім'ям і вимірами. Гетери повертають посилання на свої поля — це швидко, але
// посилання живе не довше за сам Sensor.
class Sensor {
public:
    explicit Sensor(const std::string& name);
    ~Sensor();

    const std::string& name() const;
    const std::vector<double>& samples() const;
    void add(double sample);

private:
    std::string name_;
    std::vector<double> samples_;
};
