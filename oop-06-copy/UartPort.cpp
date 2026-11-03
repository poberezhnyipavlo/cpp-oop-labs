#include "UartPort.h"

#include <iostream>

UartPort::UartPort(int port) : port_(port) {
    std::cout << "      [open] UART<" << port_ << ">" << std::endl;
}

UartPort::~UartPort() {
    std::cout << "      [close] UART<" << port_ << ">" << std::endl;
}

int UartPort::port() const {
    return port_;
}
