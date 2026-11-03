#include <iostream>

#include "Message.h"
#include "UartPort.h"

void messageCopies() {
    Message hello{"hello"};
    Message copy = hello;
    Message other{"bye"};
    Message& alias = other;
    other = alias;
    std::cout << "Message copies: " << Message::copies() << std::endl;
}

// Копія UartPort закрила б порт у своєму деструкторі, а оригінал далі працював би з уже закритим портом
// (і закрив би його вдруге). Один фізичний ресурс — один власник, тому копіювання заборонене.
void sendHello(UartPort& port) {
    std::cout << "UART<" << port.port() << ">: hello" << std::endl;
}

void nonCopyable() {
    UartPort port{100};

    sendHello(port);
}

int main() {
    // messageCopies();
    nonCopyable();
    return 0;
}