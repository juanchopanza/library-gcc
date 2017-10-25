#include "PatronService.h"
#include "PatronAccess.h"
#include "Patron.h"
#include "CreditVerifier.h"
#include "Emailer.h"

#include <vector>

using namespace std;

PatronService::PatronService() {}

PatronService::~PatronService(void) {}

/* static */ void PatronService::deleteAll() {
    PatronAccess::deleteAll();
}

vector<Patron> PatronService::getAll() const {
    return mPatronAccess.getAll();
}

void PatronService::add(const string& name, const string& cardNumber) {
    Patron patron(name, cardNumber);
    add(patron);
}

void PatronService::add(const Patron& patron) {
    if (!mVerifier)
        mPatronAccess.save(patron);
    else if (mVerifier->hasCredit(patron.creditCardNumber()))
    {
        mPatronAccess.save(patron);
    } else
    {
        if (mEmailer) mEmailer->send("", "", "", "");
    }
}

void PatronService::update(const Patron& patron) {
    mPatronAccess.update(patron);
}

bool PatronService::find(Patron& patron) const {
    return mPatronAccess.find(patron);
}

Patron PatronService::findByCardNumber(const string& cardNumber) const {
    return mPatronAccess.findByCardNumber(cardNumber);
}

Patron PatronService::findByClassification(const std::string& classification) const
{
    Patron f;
    for (const auto& p : getAll())
    {
        for (const auto& holding: p.holdings())
        {
            if (classification == holding.classification())
            {
                f = p;
                break;
            }
        }
    }
    return f;
}

short PatronService::patronCount() const {
    return mPatronAccess.size();
}
