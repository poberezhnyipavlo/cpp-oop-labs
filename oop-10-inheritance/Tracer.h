#pragma once

#include <string>

class Tracer {
public:
    Tracer();
    explicit Tracer(const std::string& name);
    Tracer(const Tracer& other);
    Tracer& operator=(const Tracer& other);
    ~Tracer();

    const std::string& name() const;
    void rename(const std::string& name);
    bool isSameObject(const Tracer& other) const;

    static int alive();

private:
    std::string name_;

    inline static int alive_ = 0;
};
