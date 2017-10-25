#ifndef CREDIT_VERIFIER_H
#define CREDIT_VERIFIER_H

#include <string>

class CreditVerifier {
public:
    virtual bool hasCredit(const std::string& cardNumber) const = 0;
    virtual ~CreditVerifier() = default;
};

#endif
