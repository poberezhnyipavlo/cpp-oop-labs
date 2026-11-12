#include "EventBus.h"

#include <map>
#include <functional>

int EventBus::subscribe(std::function<void(double)> handler) {
    const int id = next_id_;
    handlers_.insert({id, std::move(handler)});

    ++next_id_;

    return id;
}

void EventBus::unsubscribe(int id) {
    handlers_.erase(id);
}

void EventBus::publish(double value) const {
    const auto snapshot = handlers_;

    for (const auto& [id, handler] : snapshot) {
        handler(value);
    }
}