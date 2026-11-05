#include "Peer.h"

#include <iostream>
#include <memory>
#include <string>

Peer::Peer(const std::string& name) : name_(name) {
    ++alive_;
    std::cout << "      [peer] " << name_ << std::endl;
}

Peer::~Peer() {
    --alive_;
    std::cout << "      [~peer] " << name_ << std::endl;
}

void Peer::connect(const std::shared_ptr<Peer>& partner) {
    partner_ = partner;
}

std::string Peer::partnerName() const {
    if (const std::shared_ptr<Peer> partner = partner_.lock()) {
        return partner->name();
    }

    return "(нікого)";
}

const std::string& Peer::name() const {
    return name_;
}

int Peer::alive() {
    return alive_;
}
