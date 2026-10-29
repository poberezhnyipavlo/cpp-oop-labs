#pragma once

#include <string>

class Tracer {
public:
    explicit Tracer(const std::string& name);
    ~Tracer();

    const std::string& name() const;

    static int alive();

private:
    std::string name_;

    inline static int alive_ = 0;
};
