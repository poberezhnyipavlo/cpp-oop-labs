#pragma once

#include <string>

// Базовий клас: будь-який пристрій на платі має ім'я і пін.
class Device {
public:
    Device(const std::string& name, int pin);
    ~Device();

    const std::string& name() const;
    int pin() const;
    std::string describe() const;

protected:
    void log(const std::string& message) const;

private:
    std::string name_;
    int pin_;
};
