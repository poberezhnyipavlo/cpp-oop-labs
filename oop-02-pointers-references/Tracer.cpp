#include "Tracer.h"

#include <iostream>

Tracer::Tracer(const std::string& name) : name_(name) {
    ++alive_;
    std::cout << " [ctor] " << name_ << std::endl;
}

Tracer::Tracer(const Tracer& other) : name_(other.name_ + "(copy)") {
    ++alive_;
    std::cout << " [copy] " << name_ << std::endl;
}

Tracer& Tracer::operator=(const Tracer& other) {
    std::cout << " [copy=]" << name_ << " <- " << other.name_ << std::endl;
    name_ = other.name_;

    return *this;
}

Tracer::~Tracer() {
    --alive_;
    std::cout << " [dtor] " << name_ << std::endl;
}

const std::string& Tracer::name() const {
    return name_;
}

void Tracer::rename(const std::string& name) {
    std::cout << " [rename] " << name_ << " -> " << name << std::endl;
    name_ = name;
}

bool Tracer::isSameObject(const Tracer& other) const {
    return this == &other;
}

int Tracer::alive() {
    return alive_;
}
