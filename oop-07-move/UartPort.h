#pragma once

class UartPort {
public:
    explicit UartPort(int port);
    UartPort(const UartPort&) = delete;
    UartPort& operator=(const UartPort&) = delete;
    UartPort(UartPort&& other) noexcept;
    UartPort& operator=(UartPort&& other) noexcept;
    ~UartPort();

    int port() const;
private:
    int port_;

    static constexpr int kInvalidPort = -1;

    bool isValidPort() const;
    void close();
};
