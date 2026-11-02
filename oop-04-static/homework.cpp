#include <iostream>
#include <ostream>

#include "AppConfig.h"
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

void lazyConfig() {
    const auto& config = appConfig();
    std::cout << "отриманий конфіг: {device=" << config.device << " port= " << config.port << "}" << std::endl;
    const auto& config2 = appConfig();
    std::cout << "отриманий конфіг: {device=" << config2.device << " port= " << config2.port << "}" << std::endl;
    const auto& config3 = appConfig();
    std::cout << "отриманий конфіг: {device=" << config3.device << " port= " << config3.port << "}" << std::endl;
}

int main() {
    // ticketMachines();
    lazyConfig();
    return 0;
}