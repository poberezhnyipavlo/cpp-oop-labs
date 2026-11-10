#include "SampleBuffer.h"

#include <cstddef>
#include <iostream>

SampleBuffer::SampleBuffer(std::size_t size) : samples_(size) {}

double& SampleBuffer::operator[](std::size_t index) {
    std::cout << "      [operator[]] index: " << index << std::endl;

    return samples_.at(index);
}

const double& SampleBuffer::operator[](std::size_t index) const {
    std::cout << "      [operator[] const] index: " << index << std::endl;

    return samples_.at(index);
}

std::size_t SampleBuffer::size() const {
    return samples_.size();
}
