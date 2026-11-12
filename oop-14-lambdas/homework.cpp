#include <iostream>
#include <ostream>
#include <cmath>
#include <algorithm>

#include "EventBus.h"
#include "TemperatureMonitor.h"

void eventBus() {
    const auto log = [](double value) { std::cout << "log: " << value << std::endl; };

    int alarms = 0;
    const auto alarm = [&alarms](double value) {
        if (value > 60) {
            alarms++;
        }
    };

    EventBus event_bus{};
    event_bus.subscribe(log);
    event_bus.subscribe(alarm);
    const int temp_id = event_bus.subscribe([&event_bus, &temp_id](double value) {
        std::cout << "temp: " << value << std::endl;
        event_bus.unsubscribe(temp_id);
    });

    event_bus.publish(21.5);
    event_bus.publish(65.0);
    event_bus.publish(70.0);

    std::cout << "alarms: " << alarms << std::endl;
}

void monitorLifetime() {
    EventBus bus;
    {
        TemperatureMonitor monitor{bus, 60};
        bus.publish(65.0);
    }
    bus.publish(80.0);
}

void sortByDistance(std::vector<double>& values, double target) {
    std::sort(values.begin(), values.end(), [target](double a, double b) {
        return std::abs(target - a) < std::abs(target - b);
    });
}

void algorithms() {
    std::vector<double> values{30.0, 18.5, 21.0, 25.0, 19.0};
    sortByDistance(values, 21.0);
    std::cout << "відсортовані: ";
    for (auto value : values) {
        std::cout << value << ", ";
    }
    std::cout << std::endl;

    int limit = 20;
    const auto count = std::count_if(values.begin(), values.end(), [limit](double value) {
        return value > limit;
    });

    std::cout << "кількість більша за " << limit << ": " << count << std::endl;
}

int main() {
    // eventBus();

    // monitorLifetime();

    algorithms();
    return 0;
}