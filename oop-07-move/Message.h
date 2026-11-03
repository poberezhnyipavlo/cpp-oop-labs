#pragma once

#include <cstddef>

class Message {
public:
    explicit Message(const char* text);
    Message(const Message& other);
    Message(Message&& other) noexcept;
    Message& operator=(const Message& other);
    Message& operator=(Message&& other) noexcept;
    ~Message();

    const char* text() const;
    std::size_t length() const;
    static int copies();
    static int moves();
private:
    char* text_ = nullptr;
    std::size_t size_ = 0;

    bool isEmpty() const;

    static int copies_;
    static int moves_;
};
