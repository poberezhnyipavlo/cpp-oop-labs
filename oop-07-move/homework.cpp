#include <iostream>
#include <utility>

#include "Message.h"

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

int main() {
    messageMoves();
    return 0;
}