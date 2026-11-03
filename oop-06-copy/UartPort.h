#pragma once

class UartPort {
public:
    explicit UartPort(int port);
    UartPort(const UartPort&) = delete;
    UartPort& operator=(const UartPort&) = delete;
    ~UartPort();

    int port() const;
private:
    int port_;
};
