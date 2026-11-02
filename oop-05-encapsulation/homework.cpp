#include <iostream>
#include <ostream>

#include "ReadingBuffer.h"
#include "Thermostat.h"

void thermostat() {
    Thermostat t;
    t.setTarget(50);

    t.setTarget(22);
    t.update(20);
    printStatus(t);

    t.update(21.8);
    printStatus(t);

    t.update(22.6);
    printStatus(t);

    t.update(22.2);
    printStatus(t);
}

void cachedMaximum() {
    ReadingBuffer buffer;
    buffer.add(10);
    auto max = buffer.maximum();
    auto average = buffer.average();
    std::cout << "max = " << max << ", average = " << average << std::endl;
    buffer.add(100);
    max = buffer.maximum();
    average = buffer.average();
    std::cout << "max = " << max << ", average = " << average << std::endl;
    max = buffer.maximum();
    average = buffer.average();
    std::cout << "max = " << max << ", average = " << average << std::endl;
    buffer.add(20);
    max = buffer.maximum();
    average = buffer.average();
    std::cout << "max = " << max << ", average = " << average << std::endl;
}

void fixLeak() {
    ReadingBuffer buffer;
    buffer.add(20.0);
    buffer.add(22.0);
    buffer.add(24.0);
    buffer.average();
    dumpRaw(buffer);
    buffer.set(0, 100);
    buffer.average();
    dumpRaw(buffer);

    buffer.set(0, -300);
    buffer.average();
    dumpRaw(buffer);
}

int main() {
    // thermostat();
    // cachedMaximum();
    fixLeak();
    return 0;
}