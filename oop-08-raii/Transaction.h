#pragma once

#include <string>

class Transaction {
public:
    explicit Transaction(const std::string& name);
    ~Transaction();

    Transaction(Transaction const&) = delete;
    Transaction& operator=(Transaction const&) = delete;

    Transaction(Transaction&& other) = delete;
    Transaction& operator=(Transaction&& other) = delete;

    void commit();
private:
    std::string name_;
    bool committed_ = false;
};
