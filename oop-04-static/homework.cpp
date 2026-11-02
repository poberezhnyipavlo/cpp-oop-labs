#include <iostream>
#include <ostream>
#include <string>

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

namespace {
std::string formatId(int id) {
    return "A-" + std::to_string(id);
}

void linkageDemo() {
    const std::string s = formatId(1);
    std::cout << s << std::endl;
}
} // namespase
int main() {
    // ticketMachines();
    // lazyConfig();
    linkageDemo();
    return 0;
}