#pragma once

#include <string>

struct Emailer
{
    virtual void send(
        const std::string &recipient, const std::string &sender,
        const std::string &title, const std::string &content) const = 0;

    virtual ~Emailer() = default;
};