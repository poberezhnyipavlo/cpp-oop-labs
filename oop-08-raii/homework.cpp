#include <filesystem>
#include <iostream>
#include <iterator>
#include <vector>

#include "FrameBuffer.h"
#include "Socket.h"
#include "Transaction.h"

namespace {
constexpr int kSmallFrame = 8;
}

const char* yesNo(bool value) {
    return value ? "так" : "ні";
}

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
        std::cout << "створення одного сокета в середині блоку: +" << openDescriptors() - base
                  << " дескрипторів" << std::endl;
    }
    std::cout << "після блок: " << openDescriptors() - base << " дескрипторів" << std::endl;

    std::vector<Socket> sockets;

    sockets.reserve(3);
    for (int i = 0; i < 3; ++i) {
        sockets.emplace_back();
    }
    std::cout << "після додання у вектор: " << openDescriptors() - base << " дескрипторів"
              << std::endl;
}

void ruleOfZeroBuffer() {
    FrameBuffer a{"a", kSmallFrame};
    a.fill(1);

    FrameBuffer b = a;
    b.setPixel(0, 9);

    std::cout << "перший елемент: a = " << int(a.pixel(0)) << ", b = " << int(b.pixel(0))
              << std::endl;
    std::cout << "розмір: a = " << a.size() << ", b = " << b.size() << std::endl;
    std::cout << "a i b спільна пам'ять? " << yesNo(a.sharesMemoryWith(b)) << std::endl;

    FrameBuffer c = std::move(b);

    std::cout << "перший елемент: c = " << int(c.pixel(0)) << std::endl;
    std::cout << "розмір: b = " << b.size() << ", c = " << c.size() << std::endl;
    std::cout << "b - c спільна пам'ять? " << yesNo(b.sharesMemoryWith(c)) << std::endl;
}

Transaction makeTxn() {
    return Transaction{"made"};
}

void transactionScope() {
    {
        Transaction tx = makeTxn();
        tx.commit();
        tx.commit();
    }

    {
        Transaction tx{"first"};
        tx.commit();
    }

    {
        try {
            Transaction tx{"fails"};
            throw std::runtime_error{"error"};
            tx.commit();
        } catch (const std::runtime_error& e) {
            std::cout << "спіймано: " << e.what() << std::endl;
        }
    }

    {
        Transaction tx{"forgot"};
    }
}

int main() {
    // long base = openDescriptors();
    // socketGuard();
    // std::cout << "в кінці main: +" << openDescriptors() - base << " дескрипторів" << std::endl;
    // ruleOfZeroBuffer();
    transactionScope();
    return 0;
}