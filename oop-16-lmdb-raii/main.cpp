#include <lmdb.h>

#include <cerrno>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "Lmdb.h"

namespace {
constexpr unsigned int kNoNamedTables = 0;
constexpr std::size_t kMapSize = std::size_t{1} << 20U;

// A fresh LMDB file in the temp directory: data file and lock file from the last run removed.
std::filesystem::path freshFile(std::string_view name) {
    const std::filesystem::path file =
        std::filesystem::temp_directory_path() / ("oop16_" + std::string(name) + ".lmdb");
    std::filesystem::remove(file);
    std::filesystem::remove(file.string() + "-lock");
    return file;
}

// The unnamed main table: works without mdb_env_set_maxdbs.
MDB_dbi mainTable(const LmdbTxn& txn) {
    MDB_dbi dbi = 0;
    lmdbCheck(mdb_dbi_open(txn.get(), nullptr, 0, &dbi), "mdb_dbi_open");
    return dbi;
}

void put(const LmdbTxn& txn, std::string_view key, std::string_view value) {
    MDB_val k = toVal(key);
    MDB_val v = toVal(value);
    lmdbCheck(mdb_put(txn.get(), mainTable(txn), &k, &v, 0), "mdb_put");
}

// A copy of the value: what fromVal() points to is valid only while the transaction lives.
std::optional<std::string> get(const LmdbTxn& txn, std::string_view key) {
    MDB_val k = toVal(key);
    MDB_val v{};
    const int rc = mdb_get(txn.get(), mainTable(txn), &k, &v);
    if (rc == MDB_NOTFOUND) {
        return std::nullopt;
    }
    lmdbCheck(rc, "mdb_get");
    return std::string(fromVal(v));
}

void putAndCommit(const LmdbEnv& env, std::string_view key, std::string_view value) {
    LmdbTxn txn(env, LmdbTxn::Mode::kWrite);
    put(txn, key, value);
    txn.commit();
}

std::optional<std::string> readNow(const LmdbEnv& env, std::string_view key) {
    const LmdbTxn txn(env, LmdbTxn::Mode::kRead);
    return get(txn, key);
}

std::string show(const std::optional<std::string>& value) {
    return value ? *value : "(нема)";
}

std::string readSensor() {
    throw std::runtime_error("датчик не відповідає");
}

// 1. Виняток у конструкторі: об'єкта нема, а що з тим, що конструктор уже встиг створити?
void badPath() {
    try {
        const LmdbEnv env("/no/such/dir/data.lmdb", kNoNamedTables, kMapSize);
        std::cout << "відкрито?\n";
    } catch (const LmdbError& e) {
        std::cout << "LmdbError: " << e.what() << "\n";
        std::cout << "code() == ENOENT: " << (e.code() == ENOENT) << "\n";
    }
}

// 2. Перенесення: ручка переходить до іншого об'єкта, джерело лишається порожнім.
void moveHandle() {
    LmdbEnv a(freshFile("move_a"), kNoNamedTables, kMapSize);
    const MDB_env* handle = a.get();

    LmdbEnv b = std::move(a);
    std::cout << "після LmdbEnv b = std::move(a):\n";
    std::cout << "  a.get() == nullptr: " << (a.get() == nullptr) << "\n";
    std::cout << "  b.get() == handle: " << (b.get() == handle) << "\n";

    LmdbEnv c(freshFile("move_c"), kNoNamedTables, kMapSize);
    c = std::move(b);
    std::cout << "після c = std::move(b):\n";
    std::cout << "  b.get() == nullptr: " << (b.get() == nullptr) << "\n";
    std::cout << "  c.get() == handle: " << (c.get() == handle) << "\n";
}

// 3. Вектор росте: копіювати не можна, то що він робить?
void vectorGrowth() {
    std::cout << "is_copy_constructible(): " << std::is_copy_constructible_v<LmdbEnv> << "\n";
    std::cout << "is_nothrow_move_constructible: "
              << std::is_nothrow_move_assignable_v<LmdbEnv> << "\n";

    std::vector<LmdbEnv> envs;
    std::vector<const MDB_env*> handles;
    for (const char* name : {"vec_0", "vec_1", "vec_2"}) {
        envs.emplace_back(freshFile(name), kNoNamedTables, kMapSize);
        handles.push_back(envs.back().get());
        std::cout << "size " << envs.size() << ", capacity " << envs.capacity() << "\n";
    }
    for (std::size_t i = 0; i < envs.size(); ++i) {
        std::cout << "envs[" << i << "] - та сама ручка: " << (envs[i].get() == handles[i]) << "\n";
    }
}

// 4. Транзакція без commit: деструктор скасовує.
void abortWithoutCommit() {
    const LmdbEnv env(freshFile("abort"), kNoNamedTables, kMapSize);
    {
        const LmdbTxn txn(env, LmdbTxn::Mode::kWrite);
        put(txn, "a", "1");
    }
    std::cout << "без commit:   a = " << show(readNow(env, "a")) << "\n";

    try {
        LmdbTxn txn(env, LmdbTxn::Mode::kWrite);
        put(txn, "b", "2");
        put(txn, "c", readSensor());
        txn.commit();
    } catch (const std::exception& e) {
        std::cout << "виняток:      " << e.what() << "\n";
    }
    std::cout << "після винятку:    b = " << show(readNow(env, "b")) << "\n";
    putAndCommit(env, "d", "4");
    std::cout << "з commit:     d = " << show(readNow(env, "d")) << "\n";
}

// 5. Другий commit тієї самої транзакції.
void doubleCommit() {
    const LmdbEnv env(freshFile("double_commit"), kNoNamedTables, kMapSize);
    LmdbTxn txn(env, LmdbTxn::Mode::kWrite);
    put(txn, "k", "v");
    txn.commit();
    std::cout << "перший commit: txn.get() == nullptr: " << (txn.get() == nullptr) << "\n";
    try {
        txn.commit();
    } catch (const LmdbError& e) {
        std::cout << "другий commit: " << e.what() << "\n";
        std::cout << "code() == EINVAL: " << (e.code() == EINVAL) << "\n";
    }
    std::cout << "k = " << show(readNow(env, "k")) << "\n";
}

// 6. Читач бачить знімок бази на момент свого створення — поки живе об'єкт.
void snapshot() {
    const LmdbEnv env(freshFile("snapshot"), kNoNamedTables, kMapSize);
    putAndCommit(env, "temp", "21");
    {
        const LmdbTxn reader(env, LmdbTxn::Mode::kRead);
        std::cout << "reader:           temp = " << show(get(reader, "temp")) << "\n";
        {
            std::jthread writter([&env] { putAndCommit(env, "temp", "25"); });
        }
        std::cout << "новий reader:             temp = " << show(readNow(env, "temp")) << "\n";
    }
}

using Experiment = void (*)();

const std::map<std::string, Experiment> kExperiments{
    {"bad-path", badPath},
    {"move", moveHandle},
    {"vector-growth", vectorGrowth},
    {"abort", abortWithoutCommit},
    {"double-commit", doubleCommit},
    {"snapshot", snapshot},
};

void printUsage() {
    std::cout << "Використання: oop-16 <експеримент>\n";
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

    std::cout << std::boolalpha;
    kExperiments.at(argv[1])();
    return 0;
}