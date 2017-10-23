#pragma once

#include <set>
#include <string>
#include <exception>
#include <cstddef>

struct InvalidTradeException : std::exception {};

struct StockPortfolio
{
    bool empty() const { return shares_.empty(); }

    void purchase(const std::string& symbol, std::size_t quantity)
    {
        shares_[symbol] += quantity;
    }

    void sell(const std::string& symbol, std::size_t quantity)
    {
        size_t owned = shares_.at(symbol);
        if (quantity > owned)
            throw InvalidTradeException();

        shares_[symbol] -= quantity;
        if (count(symbol) == 0)
        {
            shares_.erase(symbol);
        }
    }

    std::size_t countUnique() const
    {
        return shares_.size();
    }

    std::size_t count(const std::string& symbol) const
    {
        auto it = shares_.find(symbol);
        return it != shares_.end() ? it->second : 0;
    }

 private:
    std::map<std::string, std::size_t> shares_;
};
