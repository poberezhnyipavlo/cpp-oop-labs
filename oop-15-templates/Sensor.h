#pragma once

#include <memory>
#include <string>
#include <string_view>

// ---- Динамічний поліморфізм: спільна база з virtual (oop-11) ----

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual std::string name() const = 0;
    virtual double read() const = 0;
};

class VirtualThermometer final : public ISensor {
public:
    explicit VirtualThermometer(double celsius) : celsius_(celsius) {}
    std::string name() const override {
        return "thermometer";
    }
    double read() const override {
        return celsius_;
    }

private:
    double celsius_;
};

class VirtualHygrometer final : public ISensor {
public:
    explicit VirtualHygrometer(double percent) : percent_(percent) {}
    std::string name() const override {
        return "hygrometer";
    }
    double read() const override {
        return percent_;
    }

private:
    double percent_;
};

// Одна функція для всіх нащадків ISensor; яку read() викликати — вирішується під час виконання.
void reportDynamic(const ISensor& sensor);

// Тип датчика відомий лише під час виконання (рядок з конфігурації).
std::unique_ptr<ISensor> makeSensor(std::string_view kind);

// ---- Статичний поліморфізм: жодної спільної бази, лише однакові методи ----
class Thermometer {
public:
    explicit Thermometer(double celsius) : celsius_(celsius) {}
    std::string name() const {
        return "thermometer";
    }
    double read() const {
        return celsius_;
    }
private:
    double celsius_;
};

class Hygrometer {
public:
    explicit Hygrometer(double percent) : percent_(percent) {}
    std::string name() const {
        return "hygrometer";
    }
    double read() const {
        return percent_;
    }
private:
    double percent_;
};

// Не датчик: name() є, read() нема.
class Relay {
public:
    std::string name() const {
        return "relay";
    }
};
