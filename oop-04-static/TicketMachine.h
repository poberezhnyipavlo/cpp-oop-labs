#pragma once

#include <string>

class TicketMachine {
public:
    explicit TicketMachine(const std::string& name);

    void issue();
    int issued() const;

    static int totalIssued();

private:
    std::string name_;
    int issued_ = 0;

    static int total_issued_;
};
