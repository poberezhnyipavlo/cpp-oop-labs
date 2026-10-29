#include "Tracer.h"

#include <iostream>

Tracer::Tracer(const std::string& name) : name_(name) {
    ++alive_;
    std::cout << " [ctor] " << name_ << std::endl;
}

Tracer::~Tracer() {
    --alive_;
    std::cout << " [dtor] " << name_ << std::endl;
}

const std::string& Tracer::name() const {
    return name_;
}

int Tracer::alive() {
    return alive_;
}
