#include "ReadingBuffer.h"

#include <cstddef>
#include <iostream>
#include <numeric>

bool ReadingBuffer::add(double celsius) {
    if (celsius < kMinCelsius || celsius > kMaxCelsius) {
        std::cout << "      [add] відхилено " << celsius << std::endl;
        return false;
    }

    readings_.push_back(celsius);
    invalidateCache();
    return true;
}

std::size_t ReadingBuffer::size() const {
    return readings_.size();
}

double ReadingBuffer::average() const {
    if (!cache_valid_) {
        ++recomputations_;
        std::cout << "      [recompute #" << recomputations_ << "]" << std::endl;
        const double sum = std::accumulate(readings_.begin(), readings_.end(), 0.0);
        cached_average_ = readings_.empty() ? 0.0 : sum / static_cast<double>(readings_.size());
        cache_valid_ = true;
    }
    return cached_average_;
}

double& ReadingBuffer::at(std::size_t index) {
    std::cout << "      [at]" << std::endl;
    return readings_.at(index);
}

double ReadingBuffer::at(std::size_t index) const {
    std::cout << "      [at const]" << std::endl;
    return readings_.at(index);
}

void ReadingBuffer::invalidateCache() {
    cache_valid_ = false;
}

void dumpRaw(const ReadingBuffer& buffer) {
    std::cout << "readings_ =";
    for (const double value : buffer.readings_) {
        std::cout << ' ' << value;
    }
    std::cout << ", cache_valid_ = " << buffer.cache_valid_
    << ", cached_average_ = " << buffer.cached_average_
    << ", recomputations_ = " << buffer.recomputations_
    << std::endl;
}