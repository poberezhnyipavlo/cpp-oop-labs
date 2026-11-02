#pragma once

#include <cstddef>
#include <vector>

// Буфер показів температури. Інваріант: усі значення в межах [kMinCelsius, kMaxCelsius].
class ReadingBuffer {
public:
    static constexpr double kMinCelsius = -40.0;
    static constexpr double kMaxCelsius = 125.0;

    bool add(double celsius);

    std::size_t size() const;
    double average() const;
    double maximum() const;

    bool set(std::size_t index, double value);
    double at(std::size_t index) const;

private:
    void invalidateCache();

    std::vector<double> readings_;

    mutable double cached_average_ = 0.0;
    mutable double cached_max_ = 0.0;
    mutable bool cache_valid_ = false;
    mutable bool cached_max_valid_ = false;
    mutable int recomputations_ = 0;

    friend void dumpRaw(const ReadingBuffer& buffer);
};

// Друкує внутрішній стан буфера (для налагодження). Має доступ до private, бо оголошена friend.
void dumpRaw(const ReadingBuffer& buffer);