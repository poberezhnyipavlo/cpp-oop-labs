#include <cstdio>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "Peer.h"
#include "Tracer.h"

namespace {

const char* yesNo(bool value) {
    return value ? "так" : "ні";
}

void printAlive(const std::string& where) {
    std::cout << where << ": живих Tracer: " << Tracer::alive() << std::endl;
}

std::string tempPath(const std::string& name) {
    return (std::filesystem::temp_directory_path() / name).string();
}

// Видалювач для FILE*: unique_ptr викличе його замість delete.
struct FileCloser {
    void operator()(std::FILE* handle) const {
        std::cout << "      [close] " << std::endl;
        std::fclose(handle);
    }
};

using FilePtr = std::unique_ptr<std::FILE, FileCloser>;

// Те саме звичайною функцією — для порівняння розміру.
void closeFile(std::FILE* handle) {
    std::cout << "      [fclose через функцію]" << std::endl;
    std::fclose(handle);
}

// 1. unique_ptr: єдиний власник, знищує об'єкт на }.
void uniqueBasic() {
    {
        std::unique_ptr<Tracer> sensor = std::make_unique<Tracer>("sensor");
        std::cout << "через ->: " << sensor->name() << ", через *: " << (*sensor).name() << std::endl;
        std::cout << "sizeof(unique_ptr<Tracer>) = " << sizeof(sensor) << ", sizeof(Tracer*) = " << sizeof(Tracer*) << std::endl;
        printAlive("у блоці");
    }
    printAlive("після блоку");
}

// 2. Передача власності: фабрика, std::move, функція-"поглинач".
std::unique_ptr<Tracer> makeTracer(const std::string& name) {
    return std::make_unique<Tracer>(name);
}

void consume(std::unique_ptr<Tracer> tracer) {
    std::cout << "consume: тепер я власник " << tracer->name() << std::endl;
}

void inspect(const Tracer& tracer) {
    std::cout << "inspect: лише дивлюся на " << tracer.name() << std::endl;
}

void uniqueMove() {
    std::unique_ptr<Tracer> a = makeTracer("a");
    inspect(*a);

    std::cout << "-- std::unique_ptr<Tracer> b = std::move(a);" << std::endl;
    std::unique_ptr<Tracer> b = std::move(a);
    std::cout << "a порожній? " << yesNo(a == nullptr) << ", b тримає" << b->name() << std::endl;

    std::cout << "-- consume(std::move(b));" << std::endl;
    consume(std::move(b));
    std::cout << "b порожній? " << yesNo(b == nullptr) << std::endl;

    printAlive("кінець функції");
}

// 3. Заміна, скидання і відмова від власності.
void uniqueReset() {
    std::unique_ptr<Tracer> p = std::make_unique<Tracer>("first");

    std::cout << "-- p = std::make_unique<Tracer>(\"second\");" << std::endl;
    p = std::make_unique<Tracer>("second");

    std::cout << "-- p.reset();" << std::endl;
    p.reset();
    std::cout << "p порожній? " << yesNo(!p) << std::endl;

    std::unique_ptr<Tracer> q = std::make_unique<Tracer>("released");
    std::cout << "-- Tracer* raw = q.release();" << std::endl;
    Tracer* raw = q.release();
    std::cout << "q порожній?" << yesNo(!q) << std::endl;
    printAlive("після release");

    delete raw;
    printAlive("після delete");
}

// 4. Свій видалювач: RAII для FILE* без окремого класу.
void uniqueDeleter() {
    std::cout << "sizeof(FilePtr) = " << sizeof(FilePtr) << std::endl;
    std::cout << "sizeof(unique_ptr<FILE, void (*)(FILE*)>) = "
            << sizeof(std::unique_ptr<std::FILE, void (*)(std::FILE*)>) << std::endl;

    {
        FilePtr file(std::fopen(tempPath("oop09-a.log").c_str(), "w"));
        std::fputs("hello\n", file.get());
        std::cout << "записано у файл, кінець блоку" << std::endl;
    }

    {
        std::unique_ptr<std::FILE, void (*)(std::FILE*)> file{
            std::fopen(tempPath("oop09-b.log").c_str(), "w"),
            closeFile
        };

        std::cout << "другий файл, кінець блоку" << std::endl;
    }

    FilePtr missing{std::fopen(tempPath("/no-such-dir/oop09.log").c_str(), "w")};
    std::cout << "missing порожній? " << yesNo(!missing) << std::endl;
    std::cout << "-- кінець функції" << std::endl;
}

// 5. shared_ptr: спільні власники, лічильник посилань.
void share(std::shared_ptr<Tracer> tracer) {
    std::cout << "share: use_count = " << tracer.use_count() << std::endl;
}

void sharedBasic() {
    std::shared_ptr<Tracer> config = std::make_shared<Tracer>("config");
    std::cout << "use_count = " << config.use_count() << std::endl;
    {
        std::shared_ptr<Tracer> reader = config;
        std::cout << "у блоці use_count = " << config.use_count() << ", той самий об'єкт? " << yesNo(reader.get() == config.get()) << std::endl;
        share(config);
    }
    std::cout << "після блоку use_count = " << config.use_count() << std::endl;
    std::cout << "sizeof(shared_ptr<Tracer>) = " << sizeof(config) << std::endl;

    std::cout << "-- config.reset();" << std::endl;
    config.reset();
    printAlive("після reset");
}

// 6. weak_ptr: спостерігач, який знає, чи об'єкт ще живий.
void weakObserver() {
    std::weak_ptr<Tracer> watcher;
    {
        std::shared_ptr<Tracer> config = std::make_shared<Tracer>("config");
        watcher = config;
        std::cout << "use_count = " << config.use_count() << std::endl;
        if (const std::shared_ptr<Tracer> locked = watcher.lock()) {
            std::cout << "lock(): живий, " << locked->name() << ", use_count = " << locked.use_count() << std::endl;
        }
        std::cout << "-- кінець блоку" << std::endl;
    }
    std::cout << "expired? " << yesNo(watcher.expired()) << std::endl;
    const std::shared_ptr<Tracer> locked = watcher.lock();
    std::cout << "lock() після смерті: " << (locked ? "об'єкт" : "nullptr") << std::endl;
}

// 7. Два вузли посилаються один на одного.
void cycle() {
    {
        std::shared_ptr<Peer> hub = std::make_shared<Peer>("hub");
        std::shared_ptr<Peer> node = std::make_shared<Peer>("node");
        hub->connect(node);
        node->connect(hub);
        std::cout << "hub бачить " << hub->partnerName() << ", node бачить " << node->partnerName() << std::endl;
        std::cout << "use_count: hub = " << hub.use_count() << ", node = " << node.use_count() << std::endl;
        std::cout << "-- кінець блоку" << std::endl;
    }
    std::cout << "живих Peer: " << Peer::alive() << std::endl;
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"unique-basic", uniqueBasic},
    {"unique-move", uniqueMove},
    {"unique-reset", uniqueReset},
    {"unique-deleter", uniqueDeleter},
    {"shared-basic", sharedBasic},
    {"weak-observer", weakObserver},
    {"cycle", cycle},
};

void printUsage() {
    std::cout << "Використання: oop-09 <експеримент>\n";
    for (const auto& [name, experiment] : kExperiments) {
        std::cout << "  " << name << '\n';
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2 || !kExperiments.contains(argv[1])) {
        printUsage();
        return 1;
    }

    std::cout << "main: старт\n";
    kExperiments.at(argv[1])();
    std::cout << "main: кінець, живих Tracer: " << Tracer::alive() << ", Peer: " << Peer::alive()
              << '\n';
    return 0;
}