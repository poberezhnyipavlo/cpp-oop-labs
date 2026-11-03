#include "Message.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <ostream>
#include <utility>

int Message::copies_ = 0;
int Message::moves_ = 0;

Message::Message(const char* text)
    : text_(new char[std::strlen(text) + 1]), size_(std::strlen(text)) {
    std::memcpy(this->text_, text, size_ + 1);
    std::cout << "      [ctor] " << text_ << std::endl;
}

Message::Message(const Message& other) : size_(other.size_) {
    if (!other.isEmpty()) {
        text_ = new char[other.size_ + 1];
        std::copy(other.text_, other.text_ + other.size_ + 1, text_);
    }

    ++copies_;
    std::cout << "      [copy] " << other.text() << std::endl;
}

Message::Message(Message&& other) noexcept
    : text_(std::exchange(other.text_, nullptr)), size_(std::exchange(other.size_, 0)) {
    std::cout << "      [move] " << text() << std::endl;
    ++moves_;
}

Message& Message::operator=(Message&& other) noexcept {
    std::cout << "      [move=] " << text() << " <- " << other.text() << std::endl;
    if (this == &other) {
        return *this;
    }

    delete[] text_;

    text_ = std::exchange(other.text_, nullptr);
    size_ = std::exchange(other.size_, 0);

    ++moves_;

    return *this;
}

Message& Message::operator=(const Message& other) {
    std::cout << "      [copy=] " << other.text() << std::endl;
    if (this == &other) {
        return *this;
    }
    ++copies_;
    char* fresh = nullptr;
    if (!other.isEmpty()) {
        fresh = new char[other.size_ + 1];
        std::copy(other.text_, other.text_ + other.size_ + 1, fresh);
    }

    delete[] text_;
    text_ = fresh;
    size_ = other.size_;

    return *this;
}

Message::~Message() {
    if (isEmpty()) {
        std::cout << "      [dtor] (порожній)" << std::endl;
    } else {
        std::cout << "      [dtor] " << text_ << std::endl;
    }

    delete[] text_;
}

const char* Message::text() const {
    return isEmpty() ? "" : text_;
}

std::size_t Message::length() const {
    return size_;
}

int Message::copies() {
    return copies_;
}

int Message::moves() {
    return moves_;
}

bool Message::isEmpty() const {
    return text_ == nullptr;
}