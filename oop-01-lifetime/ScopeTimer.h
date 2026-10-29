#pragma once

#include <iostream>
#include <string>
#include <chrono>

class ScopeTimer {
public:
    explicit ScopeTimer(const std::string& name)
        : name_(name), startTime_(std::chrono::steady_clock::now()) {};

    ~ScopeTimer() {
        const auto elapsed = std::chrono::steady_clock::now() - startTime_;
        const auto us = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
        std::cout << name_ << ": " << us.count() << " мкс" << std::endl;
    };

private:
    std::string name_;
    std::chrono::steady_clock::time_point startTime_;
};