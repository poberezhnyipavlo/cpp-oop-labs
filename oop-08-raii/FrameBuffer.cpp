#include "FrameBuffer.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

FrameBuffer::FrameBuffer(const std::string& name, std::size_t size)
    : name_(name), pixels_(size) {
    std::cout << "      [ctor] " << name_ << " (" << size << " bytes)" << std::endl;
}

void FrameBuffer::fill(std::uint8_t value) {
    std::fill(pixels_.begin(), pixels_.end(), value);
}

void FrameBuffer::setPixel(std::size_t index, std::uint8_t value) {
    pixels_.at(index) = value;
}

std::uint8_t FrameBuffer::pixel(std::size_t index) const {
    return pixels_.at(index);
}

std::size_t FrameBuffer::size() const {
    return pixels_.size();
}

const std::string& FrameBuffer::name() const {
    return name_;
}

bool FrameBuffer::sharesMemoryWith(const FrameBuffer& other) const {
    return  pixels_.data() == other.pixels_.data();
}
