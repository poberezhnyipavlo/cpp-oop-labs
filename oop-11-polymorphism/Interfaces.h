#pragma once

// Інтерфейси: лише чисто віртуальні методи, без полів (як interface у PHP).
class IReadable {
public:
    virtual ~IReadable() = default;
    virtual double read() const = 0;
};

class ISwitchable {
public:
    virtual ~ISwitchable() = default;
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual bool isOn() const = 0;
};