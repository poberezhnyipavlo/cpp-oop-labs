#pragma once
#include <string>

class Led {
public:
    Led(int pin, const std::string& name, int brightness);
    Led(int pin, const std::string& name);
    explicit Led(int pin);

    ~Led();

    void print() const;

    std::string name() const { return name_; }

private:
    const int pin_;
    std::string name_;
    int brightness_ = 255;
};
