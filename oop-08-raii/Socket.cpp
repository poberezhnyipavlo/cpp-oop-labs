#include "Socket.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <iostream>
#include <ostream>
#include <system_error>
#include <utility>

Socket::Socket() : fd_(::socket(AF_INET, SOCK_DGRAM, 0)) {
    if (!isValid()) {
        const int error = errno;
        throw std::system_error(error, std::generic_category(), "socket");
    }

    std::cout << "      [ctor] fd = " << fd_ << std::endl;
}

Socket::~Socket() {
    close();
}

Socket::Socket(Socket&& other) noexcept : fd_(std::exchange(other.fd_, kInvalidSocket)) {
    std::cout << "      [move] fd = " << fd_ << std::endl;
}

Socket& Socket::operator=(Socket&& other) noexcept {
    std::cout << "      [move=] fd = " << fd_ << " <- fd = " << other.fd_ << std::endl;

    if (this == &other) {
        std::cout << "не перенесено однакові" << std::endl;
        return *this;
    }

    close();

    fd_ = std::exchange(other.fd_, kInvalidSocket);

    return *this;
}

int Socket::fd() const {
    return fd_;
}

bool Socket::isValid() const {
    return fd_ != kInvalidSocket;
}

void Socket::close() {
    if (isValid()) {
        ::close(fd_);
        std::cout << "      [close] fd = " << fd_ << std::endl;
        fd_ = kInvalidSocket;
    } else {
        std::cout << "      [close] (нічого, перенесено)" << std::endl;
    }
}
