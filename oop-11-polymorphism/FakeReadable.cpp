#include "FakeReadable.h"

FakeReadable::FakeReadable(double value) : value_(value) {}

double FakeReadable::read() const {
    ++reads_;

    return value_;
}

int FakeReadable::reads() const {
    return reads_;
}
