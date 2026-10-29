#pragma once

#include "Tracer.h"

class Car {
public:
    Car();
    ~Car();

private:
    Tracer engine_;
    Tracer wheels_;
};
