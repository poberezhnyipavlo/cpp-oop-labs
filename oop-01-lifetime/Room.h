#pragma once
#include <string>

class Room {
public:
    Room(const std::string& door, const std::string& window, const std::string& floor)
        : door_(door), window_(window), floor_(floor) {}
private:
    std::string door_;
    std::string window_;
    std::string floor_;
};
