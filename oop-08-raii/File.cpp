#include "File.h"

#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

File::File(const std::string& path) : path_(path), handle_(std::fopen(path.c_str(), "w")) {
    if (handle_ == nullptr) {
        std::cout << "      [open failed] " << path_ << std::endl;
        throw std::runtime_error{"cannot open " + path_};
    }

    ++open_count_;
    std::cout << "     [open] " << path_ << std::endl;
}

File::File(File&& other) noexcept
    : path_(std::move(other.path_)), handle_(std::exchange(other.handle_, nullptr)) {
    std::cout << "      [move] " << path_ << std::endl;
}

File& File::operator=(File&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    close();
    path_ = std::move(other.path_);
    handle_ = std::exchange(other.handle_, nullptr);

    return *this;
}

File::~File() {
    close();
}

void File::write(const std::string& text) {
    std::fputs(text.c_str(), handle_);
}

bool File::isOpen() const {
    return handle_ != nullptr;
}

const std::string& File::path() const {
    return path_;
}

int File::openCount() {
    return open_count_;
}

void File::close() {
    if (handle_ == nullptr) {
        return;
    }

    std::fclose(handle_);
    handle_ = nullptr;
    --open_count_;
    std::cout << "     [close] " << path_ << std::endl;
}
