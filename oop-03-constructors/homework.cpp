#include <iostream>

#include "Led.h"

namespace {
void ledConstructors() {
    std::cout << "ledConstructors" << std::endl;

    Led a{1, "all parameters", 150};
    a.print();

    Led b{2, "two parameters"};
    b.print();

    Led c{3};
    c.print();
}
}  // namespace

int main() {
    ledConstructors();
    return 0;
}
