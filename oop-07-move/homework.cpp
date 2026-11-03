#include <iostream>
#include <utility>

#include "Message.h"
#include "UartPort.h"

void messageMoves() {
    Message a{"hello"};
    Message b = std::move(a);

    std::cout << "a після b = std::move(a): text = " << a.text() << ", length = " << a.length() << std::endl;
    std::cout << "b після b =  std::move(a): text = " << b.text() << ", length = " << b.length() << std::endl;

    Message c{"bye"};
    c = std::move(b);

    std::cout << "кількість копіювання Message: " << Message::copies() << std::endl;
    std::cout << "кількість перенесень Message: " << Message::moves() << std::endl;
}

UartPort openConsole() {
    return UartPort{20};
}

void uartTransfer() {
    UartPort console = openConsole();

    std::vector<UartPort> ports;
    ports.push_back(std::move(console));
}

void noexceptMatters() {
    std::vector<Message> messages;

    for (int i = 0; i < 10; i++) {
        messages.push_back(Message{"msg"});
    }

    std::cout << "кількість копій: " << Message::copies() << std::endl;
    std::cout << "кількість перенесень: " << Message::moves() << std::endl;
}

int main() {
    // messageMoves();
    // uartTransfer();

    noexceptMatters();
    return 0;
}