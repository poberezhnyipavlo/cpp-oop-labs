#pragma once

#include <cstddef>
#include <vector>

// Буфер вимірів з operator[] у двох версіях: для читання і для запису.
class SampleBuffer {
public:
    explicit SampleBuffer(std::size_t size);

    double& operator[](std::size_t index);
    const double& operator[](std::size_t index) const;

    std::size_t size() const;

private:
    std::vector<double> samples_;
};
