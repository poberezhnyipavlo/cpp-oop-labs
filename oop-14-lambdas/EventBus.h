#pragma once

#include <map>
#include <functional>

class EventBus {
public:
    int subscribe(std::function<void(double)> handler);
    void unsubscribe(int id);
    void publish(double value) const;
private:
    std::map<int, std::function<void(double)>> handlers_;
    int next_id_ = 1;
};
