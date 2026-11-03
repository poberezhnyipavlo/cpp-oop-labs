#include "FrameBuffer.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

FrameBuffer::FrameBuffer(const std::string& name, std::size_t size)
    : name_(name), size_(size), pixels_(new std::uint8_t[size_]{}) {
    std::cout << "      [ctor] " << name_ << " (" << size << " bytes)" << std::endl;
}

FrameBuffer::FrameBuffer(const FrameBuffer& other)
    : name_(other.name_ + "(copy)"), size_(other.size_), pixels_(new std::uint8_t[other.size_]) {
    std::copy(other.pixels_, other.pixels_ + other.size_, pixels_);
    std::cout << "      [copy] " << name_ << std::endl;
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer other) {
    std::cout << "      [copy=] " << name_ << std::endl;
    std::swap(name_, other.name_);
    std::swap(size_, other.size_);
    std::swap(pixels_, other.pixels_);

    return *this;
}

FrameBuffer::~FrameBuffer() {
    std::cout << "      [dtor] " << name_ << std::endl;
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

bool FrameBuffer::sharesMemoryWith(const FrameBuffer& other) const {
    return  pixels_ == other.pixels_;
}
