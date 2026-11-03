#include <iostream>

#include "Message.h"

void messageCopies() {
    Message hello{"hello"};
    Message copy = hello;
    Message other{"bye"};
    Message& alias = other;
    other = alias;
    std::cout << "Message copies: " << Message::copies() << std::endl;
}

int main() {
    messageCopies();
    return 0;
}