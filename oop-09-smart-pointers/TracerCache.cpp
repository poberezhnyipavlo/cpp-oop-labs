#include "TracerCache.h"

#include <iostream>
#include <ostream>
#include <string>
#include <memory>

#include "Tracer.h"

std::shared_ptr<Tracer> TracerCache::get(const std::string &name) {
    if (const std::shared_ptr<Tracer> find = cache_[name].lock()) {
        std::cout << "   [hit] " << find->name() << std::endl;
        return find;
    }

    std::cout << "      [miss] " << name << std::endl;
    std::shared_ptr<Tracer> new_tracer = std::make_shared<Tracer>(name);
    cache_[name] = new_tracer;

    return new_tracer;
}
