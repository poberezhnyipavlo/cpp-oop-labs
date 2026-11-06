#pragma once

#include <vector>

#include "Tracer.h"

class SensorRegistry {
public:
    void add(std::unique_ptr<Tracer> sensor);

    Tracer* find(const std::string& name) const;
    std::unique_ptr<Tracer> take(const std::string& name);

    std::size_t size() const;
private:
    std::vector<std::unique_ptr<Tracer>> sensors_;
};
