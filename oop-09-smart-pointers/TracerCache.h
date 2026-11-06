#pragma once

#include <string>
#include <map>

#include "Tracer.h"

class TracerCache {
public:
    std::shared_ptr<Tracer> get(const std::string& name);
private:
    std::map<std::string, std::weak_ptr<Tracer>> cache_;
};
