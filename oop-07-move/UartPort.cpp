#include "UartPort.h"

#include <iostream>
#include <utility>

UartPort::UartPort(int port) : port_(port) {
    std::cout << "      [open] UART<" << port_ << ">" << std::endl;
}

UartPort::~UartPort() {
    close();
}

UartPort::UartPort(UartPort&& other) noexcept : port_(std::exchange(other.port_, kInvalidPort)) {
    std::cout << "      [move] UART<" << port_ << ">" << std::endl;
}

UartPort& UartPort::operator=(UartPort&& other) noexcept {
    std::cout << "      [move=] UART<" << port_ << ">" << " <- UART<" << other.port_ << ">"
              << std::endl;

    if (this == &other) {
        return *this;
    }

    close();

    port_ = std::exchange(other.port_, kInvalidPort);
    return *this;
}

int UartPort::port() const {
    return port_;
}

bool UartPort::isValidPort() const {
    return port_ != kInvalidPort;
}

void UartPort::close() {
    if (isValidPort()) {
        std::cout << "      [close] UART<" << port_ << ">" << std::endl;
        port_ = kInvalidPort;
    } else {
        std::cout << "      [close] (нічого, перенесено)" << std::endl;
    }
}
