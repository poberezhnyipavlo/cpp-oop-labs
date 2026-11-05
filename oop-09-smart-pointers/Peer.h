#pragma once

#include <memory>
#include <string>

// Вузол мережі, що знає свого партнера. Живе лише всередині std::shared_ptr.
class Peer {
public:
    explicit Peer(const std::string& name);
    ~Peer();

    Peer(const Peer& peer) = delete;
    Peer& operator=(const Peer& peer) = delete;

    void connect(const std::shared_ptr<Peer>& partner);
    std::string partnerName() const;
    const std::string& name() const;

    static int alive();

private:
    std::string name_;
    std::weak_ptr<Peer> partner_;

    inline static int alive_ = 0;
};
