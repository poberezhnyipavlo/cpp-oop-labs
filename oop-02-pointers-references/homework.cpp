#include <iostream>

#include "Tracer.h"

void printNames(const Tracer& from, const Tracer& to) {
    std::cout << "from renamed name: " << from.name() <<  std::endl
           << "to renamed name: " << to.name() << std::endl;
}

void renameByValue(Tracer one, Tracer two) {
    const auto temp_two_name = two.name();
    two.rename(one.name());
    one.rename(temp_two_name);
}

void renameByPointer(Tracer* one, Tracer* two) {
    if (one == nullptr || two == nullptr) {
        std::cout << "не змінено один з параметрів або обидва nullptr" << std::endl;
        return;
    }
    const auto temp_two_name = two->name();
    two->rename(one->name());
    one->rename(temp_two_name);
}

void renameByRef(Tracer& one, Tracer& two) {
    const auto temp_two_name = two.name();
    two.rename(one.name());
    one.rename(temp_two_name);
}


void swapNames() {
    {
        std::cout << std::endl << "renameByValue" << std::endl;
        Tracer from{"from"};
        Tracer to{"to"};
        renameByValue(from, to);
        printNames(from, to);
    }
    {
        std::cout << std::endl << "renameByPointer" << std::endl;
        Tracer from{"from"};
        Tracer to{"to"};
        renameByPointer(&from, &to);
        printNames(from, to);
    }
    {
        std::cout << std::endl << "renameByRef" << std::endl;
        Tracer from{"from"};
        Tracer to{"to"};
        renameByRef(from, to);
        printNames(from, to);
    }
}

int main() {
    swapNames();
}