#include "Lmdb.h"

#include <lmdb.h>

#include <cstddef>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace {
constexpr unsigned int kMode = 0644;

unsigned int txnFlags(LmdbTxn::Mode mode) {
    if (mode == LmdbTxn::Mode::kWrite) {
        return 0U;
    }

    return MDB_RDONLY;
}
}  // namespace

LmdbError::LmdbError(const std::string& what, int code)
    : runtime_error(what + ": " + mdb_strerror(code)), code_(code) {}

int LmdbError::code() const {
    return code_;
}

void lmdbCheck(int rc, const std::string& what) {
    if (rc != MDB_SUCCESS) {
        throw LmdbError(what, rc);
    }
}

MDB_val toVal(std::string_view bytes) {
    return MDB_val{.mv_size = bytes.size(), .mv_data = const_cast<char*>(bytes.data())};
}

std::string_view fromVal(const MDB_val& val) {
    return {static_cast<const char*>(val.mv_data), val.mv_size};
}

LmdbEnv::LmdbEnv(
    const std::filesystem::path& path,
    unsigned int max_dbs,
    std::size_t map_size_bytes
) {
    lmdbCheck(mdb_env_create(&env_), "mdb_env_create");
    try {
        lmdbCheck(mdb_env_set_maxdbs(env_, max_dbs), "mdb_env_set_maxdbs");
        lmdbCheck(mdb_env_set_mapsize(env_, map_size_bytes), "mdb_env_set_mapsize");
        lmdbCheck(
            mdb_env_open(env_, path.c_str(), MDB_NOSUBDIR, kMode),
            "mdb_env_open " + path.string()
        );
    } catch (...) {
        mdb_env_close(env_);
        throw;
    }
}

LmdbEnv::~LmdbEnv() {
    if (env_ != nullptr) {
        mdb_env_close(env_);
    }
}

LmdbEnv::LmdbEnv(LmdbEnv&& other) noexcept : env_(std::exchange(other.env_, nullptr)) {}

LmdbEnv& LmdbEnv::operator=(LmdbEnv&& other) noexcept {
    if (this != &other) {
        if (env_ != nullptr) {
            mdb_env_close(env_);
        }

        env_ = std::exchange(other.env_, nullptr);
    }

    return *this;
}

MDB_env* LmdbEnv::get() const {
    return env_;
}

LmdbTxn::LmdbTxn(const LmdbEnv& env, Mode mode) {
    const unsigned int flags = txnFlags(mode);
    lmdbCheck(mdb_txn_begin(env.get(), nullptr, flags, &txn_), "mdb_txn_begin");
}

LmdbTxn::~LmdbTxn() {
    if (txn_ != nullptr) {
        mdb_txn_abort(txn_);
    }
}

void LmdbTxn::commit() {
    const int rc = mdb_txn_commit(std::exchange(txn_, nullptr));
    lmdbCheck(rc, "mdb_txn_commit");
}

MDB_txn* LmdbTxn::get() const {
    return txn_;
}