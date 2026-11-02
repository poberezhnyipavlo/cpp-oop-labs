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
int main() {
    thermostat();
    return 0;
}