#pragma once

#include <cstddef>
#include <vector>
#include "wallet/coin.hpp"

class Wallet {
public:
    void hello() const;

    void add_coin(const wallet::Coin& coin);
    std::size_t coin_count() const;
    bool has_coins() const;

    const wallet::Coin& get_coin(std::size_t index) const;
    wallet::Coin& get_coin(std::size_t index);

    bool is_coin_spent(std::size_t index) const;
    void mark_coin_spent(std::size_t index);

    void print_all_coins() const;

private:
    std::vector<wallet::Coin> coins_;
};