#include <iostream>
#include <ostream>

#include "TicketMachine.h"

void ticketMachines() {
    TicketMachine first{"first"};
    TicketMachine second{"second"};

    first.issue();
    first.issue();
    first.issue();
    second.issue();
    second.issue();
    first.issue();

    std::cout << "перша машина видала білетів: " << first.issued() << std::endl;
    std::cout << "друга машина видала білетів: " << second.issued() << std::endl;

    std::cout << "загальна кількість виданих білетів: " << TicketMachine::totalIssued() << std::endl;
}

int main() {
    ticketMachines();
    return 0;
}