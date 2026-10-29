#include "ScopeTimer.h"

void busyWork() {
    volatile long sum = 0;
    for (long i = 0; i < 10'000'000; i++) {
        sum += i;
    }
}

void nestedTimers() {
    ScopeTimer outer{"outer"};
    busyWork();

    {
        ScopeTimer inner{"inner"};
        busyWork();
    }

    busyWork();
}

int main() {
    std::cout << "Starting ..." << std::endl;
    nestedTimers();
    std::cout << "ending ..." << std::endl;
}
