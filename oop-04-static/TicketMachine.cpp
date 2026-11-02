#include "TicketMachine.h"

#include <iostream>
#include <string>

int TicketMachine::total_issued_ = 0;

TicketMachine::TicketMachine(const std::string& name) : name_(name) {
    std::cout << "      [ctor] " << name_ << std::endl;
}

int TicketMachine::issued() const {
    return issued_;
}

void TicketMachine::issue() {
    ++total_issued_;
    ++issued_;
    std::cout << name_ << ": ticket " << total_issued_ << std::endl;
}

int TicketMachine::totalIssued() {
    return total_issued_;
}