#pragma once

#include <cstdio>
#include <string>

// RAII-обгортка над FILE* з C: відкриває в конструкторі, закриває в деструкторі.
class File {
public:
    explicit File(const std::string& path);
    File(const File& other) = delete;
    File& operator=(const File& other) = delete;
    File(File&& other) noexcept;
    File& operator=(File&& other) noexcept;
    ~File();

    void write(const std::string& text);
    bool isOpen() const;
    const std::string& path() const;

    static int openCount();
private:
    void close();

    std::string path_;
    std::FILE* handle_ = nullptr;

    inline static int open_count_ = 0;
};
