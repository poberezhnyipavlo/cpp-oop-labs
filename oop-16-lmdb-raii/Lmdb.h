#pragma once

#include <lmdb.h>

#include <cstddef>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>

class LmdbError : public std::runtime_error {
public:
    LmdbError(const std::string& what, int code);

    [[nodiscard]] int code() const;

private:
    int code_;
};

void lmdbCheck(int rc, const std::string& what);
MDB_val toVal(std::string_view bytes);
std::string_view fromVal(const MDB_val& val);

class LmdbEnv {
public:
    LmdbEnv(const std::filesystem::path& path, unsigned int max_dbs, std::size_t map_size_bytes);
    ~LmdbEnv();

    LmdbEnv(const LmdbEnv&) = delete;
    LmdbEnv& operator=(const LmdbEnv&) = delete;

    LmdbEnv(LmdbEnv&& other) noexcept;
    LmdbEnv& operator=(LmdbEnv&& other) noexcept;

    [[nodiscard]] MDB_env* get() const;

private:
    MDB_env* env_ = nullptr;
};

class LmdbTxn {
public:
    enum class Mode {
        kRead,
        kWrite,
    };

    LmdbTxn(const LmdbEnv& env, Mode mode);
    ~LmdbTxn();

    LmdbTxn(const LmdbTxn&) = delete;
    LmdbTxn& operator=(const LmdbTxn&) = delete;

    LmdbTxn(LmdbTxn&&) = delete;
    LmdbTxn& operator=(LmdbTxn&&) = delete;

    void commit();

    [[nodiscard]] MDB_txn* get() const;

private:
    MDB_txn* txn_ = nullptr;
};