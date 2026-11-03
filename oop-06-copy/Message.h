#pragma once

#include <cstddef>

class Message {
public:
    explicit Message(const char* text);
    Message(const Message& other);
    Message& operator=(const Message& other);
    ~Message();

    const char* text() const;
    std::size_t length() const;
    static int copies();
private:
    char* text_;
    std::size_t size_;

    static int copies_;
};
