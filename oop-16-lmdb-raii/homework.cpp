#include <lmdb.h>

#include <filesystem>
#include <iostream>
#include <memory>

#include "LabLmdb.h"
#include "Lmdb.h"

using lab::freshFile;
using lab::get;
using lab::mainTable;
using lab::put;

constexpr unsigned int kNoNamedTables = 0;
constexpr std::size_t kMapSize = std::size_t{1} << 20U;
constexpr unsigned int kMode = 0644;

struct EnvCloser {
    void operator()(MDB_env* env) const {
        if (env != nullptr) {
            mdb_env_close(env);
        }
    }
};

class UniqueEnv {
public:
    UniqueEnv(const std::filesystem::path& path, unsigned int max_dbs, std::size_t map_size_bytes) {
        MDB_env* raw = nullptr;
        lmdbCheck(mdb_env_create(&raw), "mdb_env_create");
        env_.reset(raw);
        lmdbCheck(mdb_env_set_maxdbs(env_.get(), max_dbs), "mdb_env_set_maxdbs");
        lmdbCheck(mdb_env_set_mapsize(env_.get(), map_size_bytes), "mdb_env_set_mapsize");
        lmdbCheck(
            mdb_env_open(env_.get(), path.c_str(), MDB_NOSUBDIR, kMode),
            "mdb_env_open " + path.string()
        );
    }

    [[nodiscard]] MDB_env* get() const {
        return env_.get();
    }

private:
    std::unique_ptr<MDB_env, EnvCloser> env_;
};

void uniqueEnvRuleOfZero() {
    std::cout << "sizeof(LmdbEnv) = " << sizeof(LmdbEnv)
              << ", sizeof(UniqueEnv) = " << sizeof(UniqueEnv) << "\n";
    std::cout << "std::is_copy_constructible_v<UniqueEnv> = "
              << std::is_copy_constructible_v<UniqueEnv> << "\n";
    std::cout << "std::is_nothrow_move_constructible_v<UniqueEnv> = "
              << std::is_nothrow_move_constructible_v<UniqueEnv> << "\n";

    try {
        const UniqueEnv bad("/no/such/dir/data.lmdb", kNoNamedTables, kMapSize);
    } catch (const LmdbError& e) {
        std::cout << "LmdbError: " << e.what() << "\n";
        std::cout << "code() == ENOENT: " << (e.code() == ENOENT) << "\n";
    }

    UniqueEnv norm(freshFile("norm"), kNoNamedTables, kMapSize);
    const MDB_env* handle = norm.get();
    UniqueEnv moved = std::move(norm);

    std::cout << "norm.get() == nullptr: " << (norm.get() == nullptr) << "\n";
    std::cout << "moved.get() == handle: " << (moved.get() == handle) << "\n";

    std::cout << sizeof(std::unique_ptr<MDB_env, EnvCloser>) << '\n';
    std::cout << sizeof(std::unique_ptr<MDB_env, decltype(&mdb_env_close)>) << '\n';
}

struct CursorCloser {
    void operator()(MDB_cursor* cursor) const {
        if (cursor != nullptr) {
            mdb_cursor_close(cursor);
        }
    };
};

class LmdbCursor {
public:
    LmdbCursor(const LmdbTxn& txn, MDB_dbi dbi) {
        MDB_cursor* cursor = nullptr;
        lmdbCheck(mdb_cursor_open(txn.get(), dbi, &cursor), "mdb_cursor_open");
        cursor_.reset(cursor);
    }

    [[nodiscard]] MDB_cursor* get() const {
        return cursor_.get();
    }

private:
    std::unique_ptr<MDB_cursor, CursorCloser> cursor_;
};

void lmdbCursor(const LmdbCursor& cursor) {
    MDB_val data{};
    MDB_val key{};

    int rc = mdb_cursor_get(cursor.get(), &key, &data, MDB_FIRST);

    while (rc == MDB_SUCCESS) {
        lmdbCheck(rc, "mdb_cursor_get");
        std::string_view k(static_cast<const char*>(key.mv_data), key.mv_size);
        std::string_view v(static_cast<const char*>(data.mv_data), data.mv_size);

        std::cout << k << ": " << v << "\n";
        rc = mdb_cursor_get(cursor.get(), &key, &data, MDB_NEXT);
    }
}

void cursorWalk() {
    const LmdbEnv env(freshFile("cursor"), kNoNamedTables, kMapSize);
    {
        LmdbTxn txn_writer(env, LmdbTxn::Mode::kWrite);

        put(txn_writer, "10", "x");
        put(txn_writer, "9", "x");
        put(txn_writer, "100", "x");
        put(txn_writer, "2", "x");

        txn_writer.commit();
    }

    {
        const LmdbTxn txn_reader(env, LmdbTxn::Mode::kRead);
        const LmdbCursor cursor(txn_reader, mainTable(txn_reader));

        lmdbCursor(cursor);
    }

    std::cout << "\n\n";

    {
        LmdbTxn txn_writer(env, LmdbTxn::Mode::kWrite);
        {
            const LmdbCursor cursor(txn_writer, mainTable(txn_writer));
            put(txn_writer, "1", "x");

            lmdbCursor(cursor);
        }
        txn_writer.commit();
        std::cout << "commit пройшов\n";
    }
}

template <class Body>
void withWrittenTxn(const LmdbEnv& env, Body&& body) {
    LmdbTxn writter(env, LmdbTxn::Mode::kWrite);
    body(writter);
    writter.commit();
}

int readInt(const LmdbTxn& txn, std::string_view key) {
    std::optional<std::string> value = get(txn, key);

    if (!value.has_value()) {
        return 0;
    }

    return std::stoi(value.value());
}

void writeInt(const LmdbTxn& txn, std::string_view key, int value) {
    put(txn, key, std::to_string(value));
}

void transfer(const LmdbEnv& env, std::string_view from, std::string_view to, int amount) {
    withWrittenTxn(env, [&](const LmdbTxn& txn) {
        int to_in_bd = readInt(txn, to);
        writeInt(txn, to, to_in_bd + amount);

        int from_in_bd = readInt(txn, from);

        if (from_in_bd < amount) {
            throw std::runtime_error("недостатньо в " + std::string{from});
        }

        writeInt(txn, from, from_in_bd - amount);
    });
}

void printBalances(const LmdbEnv& env, std::string_view key_a, std::string_view key_b) {
    const LmdbTxn txn(env, LmdbTxn::Mode::kRead);

    std::cout << key_a << ": " << readInt(txn, key_a) << "\n";
    std::cout << key_b << ": " << readInt(txn, key_b) << "\n";
}

void atomicTransfer() {
    std::string key_a = "a";
    std::string key_b = "b";

    int a = 10;

    const LmdbEnv env(freshFile("transaction"), kNoNamedTables, kMapSize);

    withWrittenTxn(env, [key_a, a](const LmdbTxn& txn) { writeInt(txn, key_a, a); });

    printBalances(env, key_a, key_b);

    std::cout << "transfer 7 a in b: \n";
    try {
        transfer(env, key_a, key_b, 7);
    } catch (const std::runtime_error& e) {
        std::cout << "зловлено: " << e.what() << "\n";
    }

    printBalances(env, key_a, key_b);

    std::cout << "transfer 5 a in b: \n";
    try {
        transfer(env, key_a, key_b, 5);
    } catch (const std::runtime_error& e) {
        std::cout << "зловлено: " << e.what() << "\n";

       printBalances(env, key_a, key_b);
    }
}

int main() {
    std::cout << std::boolalpha;

    // uniqueEnvRuleOfZero();

    // cursorWalk();

    atomicTransfer();
    return 0;
}