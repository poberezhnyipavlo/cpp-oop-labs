#include <cstdio>
#include <exception>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <utility>

#include "File.h"
#include "Logger.h"

namespace {

const char* yesNo(bool value) {
    return value ? "так" : "ні";
}

// Скільки дескрипторів зараз відкрито в процесі (на Linux і macOS — записи в /dev/fd).
long openDescriptors() {
    return std::distance(
        std::filesystem::directory_iterator{"/dev/fd"},
        std::filesystem::directory_iterator{}
    );
}

std::string tempPath(const std::string& name) {
    return (std::filesystem::temp_directory_path() / name).string();
}

const std::string kBadPath = "/no-such-dir/oop8.log";

// 1. RAII: ресурс живе рівно стільки, скільки об'єкт.
void raiiBasic() {
    const long base = openDescriptors();
    {
        File file{tempPath("oop08-basic.log")};
        file.write("hello\n");
        std::cout << "усередині блоку: +" << openDescriptors() - base << " дескрипторів" << std::endl;
    }
    std::cout << "після блоку: +" << openDescriptors() - base << std::endl;
}

// 2. Ручне керування: ранній return пропускає fclose.
bool writeManually(const std::string& path, bool fail) {
    std::FILE* handle = std::fopen(path.c_str(), "w");
    if (handle == nullptr) {
        return false;
    }

    if (fail) {
        return false;
    }

    std::fputs("manual\n", handle);
    std::fclose(handle);

    return true;
}

bool writeWithRaii(const std::string& path, bool fail) {
    File file{path};
    if (fail) {
        return false;
    }
    file.write("raii\n");
    return true;
}

void manualLeak() {
    const long base = openDescriptors();
    writeManually(tempPath("oop08-manual.log"), true);
    std::cout << "після writeManually: +" << openDescriptors() - base << std::endl;
    writeWithRaii(tempPath("oop08-raii.log"), true);
    std::cout << "після writeWithRaii: +" << openDescriptors() - base << std::endl;
}

// 3. Конструктор кинув виняток: об'єкта не існує, деструктора не буде.
void ctorThrows() {
    try {
        File file{kBadPath};
        std::cout << "сюди не дійдемо" << std::endl;
    } catch (std::exception& e) {
        std::cout << "спіймано: " << e.what() << std::endl;
    }
    std::cout << "відкритих File: " << File::openCount() << std::endl;
}

// 4. Виняток на другому полі: перше поле, вже створене, знищується.
void memberCleanup() {
    try {
        Logger logger{tempPath("oop-08-main.log"), kBadPath};
        std::cout << "сюди не дійдемо" << std::endl;
    } catch (std::exception& e) {
        std::cout << "спіймано: " << e.what() << std::endl;
    }
    std::cout << "відкритих File: " << File::openCount() << std::endl;
}

// 5. Правило нуля: Logger переноситься, хоча ми не написали жодної спеціальної функції.
void ruleOfZero() {
    Logger a{tempPath("oop-08-a.log"), tempPath("oop08-a-errors.log")};
    a.info("started");

    std::cout << "-- Logger b = std::move(a);" << std::endl;
    Logger b = std::move(a);
    b.error("moved");

    std::cout << "a відкрити? " << yesNo(a.isOpen()) << ", b відкритий? " << yesNo(b.isOpen()) << std::endl;
    std::cout << "-- кінець функції" << std::endl;
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"raii-basic", raiiBasic},
    {"manual-leak", manualLeak},
    {"ctor-throws", ctorThrows},
    {"member-cleanup", memberCleanup},
    {"rule-of-zero", ruleOfZero},
};

void printUsage() {
    std::cout << "Використання: oop-08 <експеримент>\n";
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
    std::cout << "main: кінець, відкритих File: " << File::openCount() << '\n';
    return 0;
}