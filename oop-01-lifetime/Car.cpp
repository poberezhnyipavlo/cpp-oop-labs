#include "Car.h"

#include <iostream>

Car::Car() : engine_("engine"), wheels_("wheels") {
    std::cout << " [Car ctor body] \n";
}

Car::~Car() {
    std::cout << " [Car dtor body] \n";
}