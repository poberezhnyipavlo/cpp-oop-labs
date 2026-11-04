#include <filesystem>
#include <iostream>
#include <iterator>
#include <vector>

#include "Socket.h"

long openDescriptors() {
    return std::distance(
        std::filesystem::directory_iterator{"/dev/fd"},
        std::filesystem::directory_iterator{}
    );
}

Socket makeSocket() {
    return Socket();
}

void socketGuard() {
    long base = openDescriptors();
    {
        Socket s = makeSocket();
        std::cout << "створення одного сокета в середині блоку: +" << openDescriptors() - base << " дескрипторів"
                  << std::endl;
    }
    std::cout << "після блок: " << openDescriptors() - base << " дескрипторів" << std::endl;

    std::vector<Socket> sockets;

    sockets.reserve(3);
    for (int i = 0; i < 3; ++i) {
        sockets.emplace_back();
    }
    std::cout << "після додання у вектор: " << openDescriptors() - base << " дескрипторів" << std::endl;
}

int main() {
    long base = openDescriptors();
    socketGuard();
    std::cout << "в кінці main: +" << openDescriptors() - base << " дескрипторів" << std::endl;
    return 0;
}