#include "Room.h"
#include "ScopeTimer.h"
#include "Tracer.h"

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

void rawHeapArray() {
    Tracer* items[3]{};

    items[0] = new Tracer("Tracer 0");
    items[1] = new Tracer("Tracer 1");
    items[2] = new Tracer("Tracer 2");

    for (const Tracer* item : items) {
        delete item;
    }
}

void rawHeapArrayWithException() {
    Tracer* items[3]{};

    items[0] = new Tracer("Tracer 0");
    items[1] = new Tracer("Tracer 1");
    throw std::runtime_error{"шось пішло не так"};
    items[2] = new Tracer("Tracer 2");

    for (const Tracer* item : items) {
        delete item;
    }
}

void uniquePtrArray() {
    std::unique_ptr<Tracer> items[3]{};
    items[0] = std::make_unique<Tracer>("Tracer 0");
    items[1] = std::make_unique<Tracer>("Tracer 1");
    items[2] = std::make_unique<Tracer>("Tracer 3");
}

void memberOrder() {
    Room room{"door", "window", "floor"};
}

int main() {
    std::cout << "Starting ..." << std::endl;

    // try {
    //     rawHeapArrayWithException();
    // } catch (std::exception& e) {
    //     std::cout << e.what() << std::endl;
    // }

    // uniquePtrArray();

    memberOrder();

    // std::cout << "трасерів залишилося: " << Tracer::alive() << std::endl;
    std::cout << "ending ..." << std::endl;
}
