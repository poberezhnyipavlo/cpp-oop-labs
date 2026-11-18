#pragma once

#include <lmdb.h>

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

#include "Lmdb.h"

// Допоміжне для експериментів, тестів і домашнього: головна таблиця і прості put/get.
namespace lab {

// Свіжий файл LMDB у тимчасовій папці (старий файл і -lock видалено).
std::filesystem::path freshFile(std::string_view name);

// Головна безіменна таблиця: працює без mdb_env_set_maxdbs.
MDB_dbi mainTable(const LmdbTxn& txn);

void put(const LmdbTxn& txn, std::string_view key, std::string_view value);

// Копія значення: fromVal() дійсне лише поки жива транзакція.
std::optional<std::string> get(const LmdbTxn& txn, std::string_view key);

void putAndCommit(const LmdbEnv& env, std::string_view key, std::string_view value);
std::optional<std::string> readNow(const LmdbEnv& env, std::string_view key);

}  // namespace lab