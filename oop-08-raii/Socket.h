#pragma once

class Socket {
public:
    static constexpr int kInvalidSocket = -1;

    Socket();
    ~Socket();

    Socket(Socket const&) = delete;
    Socket& operator=(Socket const&) = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    int fd() const;
private:
    int fd_ = kInvalidSocket;

    bool isValid() const;
    void close();
};
