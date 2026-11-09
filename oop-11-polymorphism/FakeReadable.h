#pragma once

#include "Interfaces.h"

class FakeReadable : public IReadable {
public:
    explicit FakeReadable(double value);
    double read() const override;
    int reads() const;

private:
    double value_;
    mutable int reads_ = 0;
};
