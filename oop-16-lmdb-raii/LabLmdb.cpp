#include "LabLmdb.h"

#include <lmdb.h>

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

#include "Lmdb.h"

std::filesystem::path lab::freshFile(std::string_view name) {
    const std::filesystem::path file =
        std::filesystem::temp_directory_path() / ("oop16_" + std::string(name) + ".lmdb");
    std::filesystem::remove(file);
    std::filesystem::remove(file.string() + "-lock");
    return file;
}

MDB_dbi lab::mainTable(const LmdbTxn& txn) {
    MDB_dbi dbi = 0;
    lmdbCheck(mdb_dbi_open(txn.get(), nullptr, 0, &dbi), "mdb_dbi_open");
    return dbi;
}

void lab::put(const LmdbTxn& txn, std::string_view key, std::string_view value) {
    MDB_val k = toVal(key);
    MDB_val v = toVal(value);
    lmdbCheck(mdb_put(txn.get(), mainTable(txn), &k, &v, 0), "mdb_put");
}

std::optional<std::string> lab::get(const LmdbTxn& txn, std::string_view key) {
    MDB_val k = toVal(key);
    MDB_val v{};
    const int rc = mdb_get(txn.get(), mainTable(txn), &k, &v);
    if (rc == MDB_NOTFOUND) {
        return std::nullopt;
    }
    lmdbCheck(rc, "mdb_get");
    return std::string(fromVal(v));
}

void lab::putAndCommit(const LmdbEnv& env, std::string_view key, std::string_view value) {
    LmdbTxn txn(env, LmdbTxn::Mode::kWrite);
    put(txn, key, value);
    txn.commit();
}

std::optional<std::string> lab::readNow(const LmdbEnv& env, std::string_view key) {
    const LmdbTxn txn(env, LmdbTxn::Mode::kRead);
    return get(txn, key);
}
