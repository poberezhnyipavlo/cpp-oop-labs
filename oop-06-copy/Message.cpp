#include "Message.h"

#include <cstring>
#include <iostream>
#include <ostream>
#include <algorithm>

int Message::copies_ = 0;

Message::Message(const char* text) : text_(new char[std::strlen(text) + 1]), size_(std::strlen(text)) {
    std::memcpy(this->text_, text, size_ + 1);
    std::cout << "      [ctor] " << text_ << std::endl;
}

Message::Message(const Message& other) : text_(new char[std::strlen(other.text_) + 1]), size_(other.size_) {
    std::copy(other.text_, other.text_ + other.size_ + 1,text_);
    ++copies_;
    std::cout << "      [copy] " << other.text_ << std::endl;
}

Message& Message::operator=(const Message& other) {
    std::cout << "      [copy=] " << other.text_ << std::endl;
    if (this == &other) {
        return *this;
    }
    ++copies_;
    auto* fresh = new char[other.size_ + 1];

    std::copy(other.text_, other.text_ + other.size_ + 1, fresh);
    delete [] text_;
    text_ = fresh;
    size_ = other.size_;

    return *this;
}

Message::~Message() {
    std::cout << "      [dtor] " << text_ << std::endl;
    delete [] text_;
}

const char* Message::text() const {
    return text_;
}

std::size_t Message::length() const {
    return size_;
}

int Message::copies() {
    return copies_;
}
