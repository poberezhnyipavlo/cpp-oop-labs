#include <filesystem>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <iterator>
#include <utility>

#include "SensorRegistry.h"
#include "Tracer.h"
#include "TracerCache.h"

std::string tempPath(const std::string &name) {
    return (std::filesystem::temp_directory_path() / name).string();
}

struct FileCloser {
    void operator()(std::FILE *handle) const {
        std::cout << "      [close] " << std::endl;
        std::fclose(handle);
    }
};

long openDescriptors() {
    return std::distance(
        std::filesystem::directory_iterator{"/dev/fd"},
        std::filesystem::directory_iterator{}
    );
}

using FilePtr = std::unique_ptr<std::FILE, FileCloser>;

FilePtr openFile(const std::string &path) {
    FilePtr file(std::fopen(path.c_str(), "w"));
    if (!file) {
        throw std::runtime_error{"cannot open " + path};
    }

    return file;
}

bool writeWithUnique(const std::string &path, bool fail) {
    FilePtr file = openFile(path);

    if (fail) {
        return false;
    }

    std::fputs("writeWithUnique", file.get());

    return true;
}

void ownedFile() {
    long base = openDescriptors();
    writeWithUnique(tempPath("oop09.log"), true);
    std::cout << "після fail = true: +" << openDescriptors() - base << " дескрипторів" << std::endl;
    writeWithUnique(tempPath("oop09.log"), false);
    std::cout << "після fail = false: +" << openDescriptors() - base << " дескрипторів" << std::endl;
    try {
        openFile("/no-such-dir/x.log");
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "після exception: +" << openDescriptors() - base << " дескрипторів" << std::endl;
}

void sensorRegistry() {
    std::unique_ptr<Tracer> pir;
    {
        SensorRegistry registry;
        pir = std::make_unique<Tracer>("pir");
        registry.add(std::make_unique<Tracer>("dht"));
        registry.add(std::move(pir));
        registry.add(std::make_unique<Tracer>("light"));
        registry.add(std::make_unique<Tracer>("relay"));
        registry.add(std::make_unique<Tracer>("pot"));

        if (Tracer *dht_for_raname = registry.find("dht")) {
            dht_for_raname->rename("dht11");
        }

        Tracer *find = registry.find("none");
        std::cout << "знайдено none: " << (find != nullptr ? find->name() : "не знайдено") << std::endl;
        std::cout << "кількість registry = " << registry.size() << std::endl;
        pir = registry.take("pir");
        std::cout << "кількість після take = " << registry.size() << std::endl;
    }
    std::cout << "pir->name() = " << pir->name() << std::endl;
    std::cout << "живих Tracer: " << Tracer::alive() << std::endl;
}

void weakCache() {
    TracerCache cache{};

    std::shared_ptr<Tracer> a = cache.get("config");
    std::shared_ptr<Tracer> b = cache.get("config");

    std::cout << "a use_count: " << a.use_count() << std::endl;
    std::cout << "b use_count: " << b.use_count() << std::endl;

    if (a == b) {
        std::cout << "в памяті один обєкт" << std::endl;
    }

    a.reset();
    b.reset();

    std::cout << "кількість живих Tracer: " << Tracer::alive() << std::endl;

    std::shared_ptr<Tracer> c = cache.get("config");
}

int main() {
    // ownedFile();
    // sensorRegistry();
    weakCache();
    return 0;
}
