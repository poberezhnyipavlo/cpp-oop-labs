#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

// Буфер кадру для дисплея (як 1024 байти в SSD1306). Володіє пам'яттю в купі.
class FrameBuffer {
public:
    FrameBuffer(const std::string& name, std::size_t size);
    FrameBuffer(const FrameBuffer& other);
    FrameBuffer& operator=(FrameBuffer other);
    ~FrameBuffer();

    void fill(std::uint8_t value);
    void setPixel(std::size_t index, std::uint8_t value);
    std::uint8_t pixel(std::size_t index) const;

    std::size_t size() const;
    const std::string& name() const;
    bool sharesMemoryWith(const FrameBuffer& other) const;

private:
    std::string name_;
    std::size_t size_;
    std::uint8_t* pixels_;
};
