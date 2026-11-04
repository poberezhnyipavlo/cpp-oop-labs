#include "Transaction.h"

#include <iostream>
#include <ostream>

Transaction::Transaction(const std::string& name) : name_(name) {
    std::cout << "      [begin] " << name_ << std::endl;
}

Transaction::~Transaction() {
    if (!committed_) {
        std::cout << "      [rollback] " << name_ << std::endl;
    }
}

void Transaction::commit() {
    if (committed_) {
        return;
    }
    committed_ = true;
    std::cout << "      [commit] " << name_ << std::endl;
}