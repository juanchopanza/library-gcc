#pragma once
#include <string>

inline std::string romanize(unsigned int number)
{
    auto n = number % 10;

    if (n == 5)
    {
        return "V";
    }
    else if (n < 5)
    {
        return std::string(n, 'I');
    }
    else
    {
        auto m = n % 5;
        return "V" + std::string(m, 'I'); 
    }

}
