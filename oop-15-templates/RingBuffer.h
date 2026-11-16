#pragma once

#include <array>
#include <cstddef>
#include <stdexcept>
#include <type_traits>

template <class T, std::size_t N>
class RingBuffer {
    static_assert(N > 0, "N must be positive");
public:
    void push(const T& value) {
        if (full()) {
            head_ = (head_ + 1) % N;
            --count_;
        }

        data_[(head_ + count_) % N] = value;
        ++count_;
    }

    std::size_t size() const {
        return count_;
    }

    bool full() const {
        return count_ == N;
    }

    const T& at(std::size_t index) const {
        if (index >= count_) {
            throw std::out_of_range("RingBuffer::at: index out of range");
        }

        return data_[(index + head_) % N];
    }

private:
    std::array<T, N> data_{};
    std::size_t head_ = 0;
    std::size_t count_ = 0;
};

template <class T, std::size_t N> requires std::is_arithmetic_v<T>
double average(const RingBuffer<T, N>& buffer) {
    if (buffer.size() == 0) {
        return 0.0;
    }

    double sum = 0.0;
    for (std::size_t i = 0; i < buffer.size(); ++i) {
        sum += buffer.at(i);
    }

    return sum / static_cast<double>(buffer.size());
}