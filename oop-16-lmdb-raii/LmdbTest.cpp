#include <gtest/gtest.h>
#include <lmdb.h>

#include <cerrno>
#include <cstddef>
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "Lmdb.h"

// The contract of the types, checked by the compiler: a failed line does not compile.
static_assert(!std::is_copy_constructible_v<LmdbEnv>);
static_assert(!std::is_copy_assignable_v<LmdbEnv>);
static_assert(std::is_nothrow_move_constructible_v<LmdbEnv>);
static_assert(std::is_nothrow_move_assignable_v<LmdbEnv>);
static_assert(!std::is_copy_constructible_v<LmdbTxn>);
static_assert(!std::is_move_constructible_v<LmdbTxn>);
static_assert(std::is_base_of_v<std::runtime_error, LmdbError>);

namespace {

constexpr std::size_t kTestMapSize = std::size_t{1} << 20U;  // 1 MiB

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

std::optional<std::string> readNow(const LmdbEnv& env, std::string_view key) {
    const LmdbTxn txn(env, LmdbTxn::Mode::kRead);
    MDB_val k = toVal(key);
    MDB_val v{};
    const int rc = mdb_get(txn.get(), mainTable(txn), &k, &v);
    if (rc == MDB_NOTFOUND) {
        return std::nullopt;
    }
    lmdbCheck(rc, "mdb_get");
    return std::string(fromVal(v));
}

// Each test gets its own empty LMDB file, removed afterwards.
class LmdbTest : public ::testing::Test {
protected:
    void SetUp() override {
        const auto* info = ::testing::UnitTest::GetInstance()->current_test_info();
        file_ = std::filesystem::temp_directory_path() /
                ("oop16_test_" + std::string(info->name()) + ".lmdb");
        removeFiles();
    }

    void TearDown() override {
        removeFiles();
    }

    void removeFiles() const {
        std::filesystem::remove(file_);
        std::filesystem::remove(file_.string() + "-lock");
    }

    [[nodiscard]] LmdbEnv openEnv() const {
        return LmdbEnv(file_, 0, kTestMapSize);
    }

    std::filesystem::path file_;
};

}  // namespace

TEST(LmdbError, KeepsCodeAndExplainsIt) {
    const LmdbError error("mdb_get", MDB_NOTFOUND);
    EXPECT_EQ(error.code(), MDB_NOTFOUND);
    EXPECT_STREQ(error.what(), "mdb_get: MDB_NOTFOUND: No matching key/data pair found");
}

TEST(LmdbCheck, ThrowsOnlyOnError) {
    EXPECT_NO_THROW(lmdbCheck(MDB_SUCCESS, "ok"));
    EXPECT_THROW(lmdbCheck(EINVAL, "bad"), LmdbError);
}

// Keys in the monitor contain 0x00 bytes: the size travels with the pointer.
TEST(LmdbVal, KeepsZeroBytes) {
    const std::string bytes("\x00\x00\x00\x2a", 4);
    const MDB_val val = toVal(bytes);
    EXPECT_EQ(val.mv_size, 4U);
    EXPECT_EQ(fromVal(val), bytes);
}

TEST_F(LmdbTest, EnvCreatesDataAndLockFiles) {
    const LmdbEnv env = openEnv();
    EXPECT_NE(env.get(), nullptr);
    EXPECT_TRUE(std::filesystem::exists(file_));
    EXPECT_TRUE(std::filesystem::exists(file_.string() + "-lock"));
}

TEST_F(LmdbTest, EnvOnMissingDirectoryThrowsWithCode) {
    try {
        const LmdbEnv env("/no/such/dir/data.lmdb", 0, kTestMapSize);
        FAIL() << "expected LmdbError";
    } catch (const LmdbError& e) {
        EXPECT_EQ(e.code(), ENOENT);
        EXPECT_TRUE(std::string_view(e.what()).starts_with("mdb_env_open /no/such/dir/data.lmdb"));
    }
}

TEST_F(LmdbTest, MoveConstructorTakesHandle) {
    LmdbEnv source = openEnv();
    const MDB_env* handle = source.get();

    const LmdbEnv target = std::move(source);
    EXPECT_EQ(target.get(), handle);
    EXPECT_EQ(source.get(), nullptr);  // NOLINT(bugprone-use-after-move): checking moved-from
}

TEST_F(LmdbTest, MoveAssignmentTakesHandle) {
    LmdbEnv source = openEnv();
    const MDB_env* handle = source.get();
    LmdbEnv target(file_.string() + "_other", 0, kTestMapSize);

    target = std::move(source);
    EXPECT_EQ(target.get(), handle);
    EXPECT_EQ(source.get(), nullptr);  // NOLINT(bugprone-use-after-move): checking moved-from

    std::filesystem::remove(file_.string() + "_other");
    std::filesystem::remove(file_.string() + "_other-lock");
}

TEST_F(LmdbTest, SelfMoveAssignmentKeepsHandle) {
    LmdbEnv env = openEnv();
    const MDB_env* handle = env.get();
    LmdbEnv& same = env;  // through a reference: `env = std::move(env)` is a compiler warning

    env = std::move(same);
    EXPECT_EQ(env.get(), handle);
    EXPECT_NO_THROW(readNow(env, "any"));
}

TEST_F(LmdbTest, CommitStoresData) {
    const LmdbEnv env = openEnv();
    LmdbTxn txn(env, LmdbTxn::Mode::kWrite);
    put(txn, "temp", "21.5");
    txn.commit();

    EXPECT_EQ(txn.get(), nullptr);
    EXPECT_EQ(readNow(env, "temp"), "21.5");
}

TEST_F(LmdbTest, NoCommitMeansNoData) {
    const LmdbEnv env = openEnv();
    {
        const LmdbTxn txn(env, LmdbTxn::Mode::kWrite);
        put(txn, "temp", "21.5");
    }
    EXPECT_EQ(readNow(env, "temp"), std::nullopt);
}

TEST_F(LmdbTest, ExceptionBeforeCommitMeansNoData) {
    const LmdbEnv env = openEnv();
    EXPECT_THROW(
        {
            LmdbTxn txn(env, LmdbTxn::Mode::kWrite);
            put(txn, "temp", "21.5");
            throw std::runtime_error("sensor failed");
        },
        std::runtime_error
    );
    EXPECT_EQ(readNow(env, "temp"), std::nullopt);
}

TEST_F(LmdbTest, SecondCommitThrowsInsteadOfDoubleFree) {
    const LmdbEnv env = openEnv();
    LmdbTxn txn(env, LmdbTxn::Mode::kWrite);
    txn.commit();
    try {
        txn.commit();
        FAIL() << "expected LmdbError";
    } catch (const LmdbError& e) {
        EXPECT_EQ(e.code(), EINVAL);
    }
}

TEST_F(LmdbTest, ReadTransactionCannotWrite) {
    const LmdbEnv env = openEnv();
    const LmdbTxn txn(env, LmdbTxn::Mode::kRead);
    EXPECT_THROW(put(txn, "temp", "21.5"), LmdbError);
}

TEST_F(LmdbTest, DataSurvivesReopen) {
    {
        const LmdbEnv env = openEnv();
        LmdbTxn txn(env, LmdbTxn::Mode::kWrite);
        put(txn, "temp", "21.5");
        txn.commit();
    }
    const LmdbEnv reopened = openEnv();
    EXPECT_EQ(readNow(reopened, "temp"), "21.5");
}