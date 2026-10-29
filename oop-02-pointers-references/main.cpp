#include <iostream>
#include <map>
#include <string>
#include <utility>

#include "Tracer.h"
namespace {
const char* yesNo(bool value) {
    return value ? "так" : "ні";
}

// П'ять способів передати об'єкт у функцію.
void byValue(Tracer t) {
    std::cout << "  byValue отримав " << t.name() << std::endl;
    t.rename("changed-by-value");
}

void byPointer(Tracer* t) {
    if (t == nullptr) {
        std::cout << "  byPointer отримав nullptr" << std::endl;
        return;
    }

    std::cout << "  byPointer отримав " << t->name() << std::endl;
    t->rename("changed-by-pointer");
}

void byRef(Tracer& t) {
    std::cout << "  byRef отримав " << t.name() << std::endl;
    t.rename("changed-by-ref");
}

void byConstRef(const Tracer& t) {
    std::cout << "  byConstRef отримав " << t.name() << std::endl;
}

void byRvalueRef(Tracer&& t) {
    std::cout << "  byRvalueRef  отримав " << t.name() << std::endl;
}

// 1. Вказівник — окремий об'єкт з адресою; посилання — інше ім'я того самого об'єкта.
void addresses() {
    Tracer x{"x"};
    Tracer* p = &x;
    Tracer& r = x;

    std::cout << "&x = " << &x << std::endl;
    std::cout << "p = " << p << std::endl;
    std::cout << "&r = " << &r << std::endl;
    std::cout << "&p = " << &p << std::endl;
    std::cout << "sizeof(x) = " << sizeof(x) << ", sizeof(p) = " << sizeof(p)
              << ", sizeof(r) = " << sizeof(r) << std::endl;
    std::cout << "(*p).name() = " << (*p).name() << ", p->name() = " << p->name()
              << ", r.name() = " << r.name() << std::endl;
}

// 2. Вказівник можна перенаправити, посилання — ні.
void reseat() {
    Tracer a{"a"};
    Tracer b{"b"};

    Tracer* p = &a;
    p = &b;
    std::cout << "p тепер вказує на " << p->name() << std::endl;

    Tracer& r = a;
    std::cout << "r.name() = " << r.name() << ", a.name() = " << a.name() << std::endl;
    std::cout << "r - це досі a? " << yesNo(a.isSameObject(r)) << std::endl;
}

// 3. Передача змінної (lvalue).
void passLvalue() {
    Tracer x{"x"};

    std::cout << "-- byValue(x)" << std::endl;
    byValue(x);
    std::cout << "x = " << x.name() << std::endl;

    std::cout << "-- byPointer(&x)" << std::endl;
    byPointer(&x);
    std::cout << "x = " << x.name() << std::endl;

    std::cout << "-- byRef(x)" << std::endl;
    byRef(x);
    std::cout << "x = " << x.name() << std::endl;

    std::cout << "-- byConstRef(x)" << std::endl;
    byConstRef(x);
}

// 4. Передача тимчасового об'єкта (rvalue).
void passTemporary() {
    std::cout << "-- byValue(Tracer{\"tmp1\"})" << std::endl;
    byValue(Tracer{"tmp1"});

    std::cout << "-- byConstRef(Tracer{\"tmp2\"})" << std::endl;
    byConstRef(Tracer{"tmp2"});

    std::cout << "-- byRvalueRef(Tracer{\"tmp3\"})" << std::endl;
    byRvalueRef(Tracer{"tmp3"});

    std::cout << "-- кінець функції" << std::endl;
}

// 5. std::move нічого не переносить.
void moveIsCast() {
    Tracer x{"x"};
    byRvalueRef(std::move(x));
    std::cout << "після std::move: x = " << x.name() << std::endl;
}

// 6. Вказівник може бути порожнім.
void nullPointer() {
    byPointer(nullptr);
}

void nullDeref() {
    Tracer* p = nullptr;
    std::cout << "name = " << p->name() << std::endl;
}

// 7. this — вказівник на об'єкт, для якого викликано метод.
void thisPointer() {
    Tracer a{"a"};
    Tracer b{"b"};
    Tracer& r = a;
    Tracer* p = &a;

    std::cout << "a.isSameObject(a) = " << yesNo(a.isSameObject(a)) << std::endl;
    std::cout << "a.isSameObject(b) = " << yesNo(a.isSameObject(b)) << std::endl;
    std::cout << "a.isSameObject(r) = " << yesNo(a.isSameObject(r)) << std::endl;
    std::cout << "a.isSameObject(*p) = " << yesNo(a.isSameObject(*p)) << std::endl;
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiment{
    {"addresses", addresses},
    {"reseat", reseat},
    {"pass-lvalue", passLvalue},
    {"pass-temporary", passTemporary},
    {"move-is-cast", moveIsCast},
    {"null-pointer", nullPointer},
    {"null-deref", nullDeref},
    {"this", thisPointer},
};

void printUsage() {
    std::cout << "Використання: oop-2 <експеримент>" << std::endl;
    for (const auto& [name, experiment] : kExperiment) {
        std::cout << "  " << name << std::endl;
    }
}
}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2 || !kExperiment.contains(argv[1])) {
        printUsage();
        return 1;
    }

    std::cout << "main: старт" << std::endl;
    kExperiment.at(argv[1])();
    std::cout << "main: кінець, живих Tracer: " << Tracer::alive() << std::endl;
    return 0;
}