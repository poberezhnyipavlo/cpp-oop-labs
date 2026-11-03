#include "FrameBuffer.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

FrameBuffer::FrameBuffer(const std::string& name, std::size_t size)
    : name_(name), size_(size), pixels_(new std::uint8_t[size_]{}) {
    std::cout << "      [ctor] " << name_ << std::endl;
}

FrameBuffer::FrameBuffer(const FrameBuffer& other)
    : name_(other.name_ + "'"), size_(other.size_), pixels_(new std::uint8_t[other.size_]) {
    std::copy(other.pixels_, other.pixels_ + other.size_, pixels_);
    std::cout << "      [copy] " << name_ << std::endl;
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
    : name_(std::move(other.name_)),
    size_(std::exchange(other.size_, 0)),
    pixels_(std::exchange(other.pixels_, nullptr)) {
    std::cout << "      [move] " << name_ << std::endl;
}

FrameBuffer& FrameBuffer::operator=(const FrameBuffer& other) {
    std::cout << "      [copy=] " << name_ << " <- " << other.name_ << std::endl;
    if (this == &other) {
        return *this;
    }

    auto* fresh = new std::uint8_t[other.size_];
    std::copy(other.pixels_, other.pixels_ + other.size(), fresh);
    delete[] pixels_;
    pixels_ = fresh;
    size_ = other.size_;
    name_ = other.name_;

    return *this;
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept {
    std::cout << "      [move=] " << name_ << " <- " << other.name_ << std::endl;
    if (this == &other) {
        return *this;
    }
    delete[] pixels_;
    pixels_ = std::exchange(other.pixels_, nullptr);
    size_ = std::exchange(other.size_, 0);
    name_ = std::move(other.name_);

    return *this;

}

FrameBuffer::~FrameBuffer() {
    if (isEmpty()) {
        std::cout << "      [dtor] (порожній)" << std::endl;
    } else {
        std::cout << "      [dtor] " << name_ << std::endl;
    }
    delete[] pixels_;
}

void FrameBuffer::fill(std::uint8_t value) {
    std::fill(pixels_, pixels_ + size_, value);
}

void FrameBuffer::setPixel(std::size_t index, std::uint8_t value) {
    pixels_[index] = value;
}

std::uint8_t FrameBuffer::pixel(std::size_t index) const {
    return pixels_[index];
}

std::size_t FrameBuffer::size() const {
    return size_;
}

const std::string& FrameBuffer::name() const {
    return name_;
}

bool FrameBuffer::isEmpty() const {
    return  pixels_ == nullptr;
}
