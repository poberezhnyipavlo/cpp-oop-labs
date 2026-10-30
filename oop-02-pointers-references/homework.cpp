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

Tracer* findByName(Tracer* items[], std::size_t count, const std::string& name) {
    for (std::size_t i = 0; i < count; ++i) {
        if (items[i] && items[i]->name() == name) {
            return items[i];
        }
    }

    return nullptr;
}

void find() {
    Tracer a{"a"};
    Tracer b{"b"};
    Tracer c{"c"};

    Tracer* items[] = { &a, &b, &c };

    Tracer* found = findByName(items, std::size(items), "a");
    if (found) {
        std::cout << "знайдено: " << found->name() << std::endl;
        found->rename("renamed");
    }

    Tracer* found_renamed = findByName(items, std::size(items), "renamed");
    if (found_renamed) {
        std::cout << "знайдено: " << found->name() << std::endl;
        found->rename("renamed 2");
    }

    Tracer* not_found = findByName(items, std::size(items), "not_found");
    if (not_found == nullptr) {
        std::cout << "не знайдено not_found" << std::endl;
    }
}

const Tracer& longerName(const Tracer& one, const Tracer& two) {
    return one.name().length() > two.name().length() ? one : two;
};

void longerNameOfVariables() {
    Tracer one{"one"};
    Tracer two{"two-two"};

    const Tracer& r = longerName(one, two);

    std::cout << "найдовше імя: " << r.name() << std::endl;
}

void longerNameOfTemporaries() {
    const Tracer& r = longerName(Tracer{"one-one"}, Tracer{"two"});

    std::cout << "найдовше імя: " << r.name() << std::endl;
}

int main() {
    // swapNames();
    // find();

    // longerNameOfVariables();

    longerNameOfTemporaries();
    return 0;
}