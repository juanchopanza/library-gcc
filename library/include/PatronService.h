#ifndef PATRON_SERVICE_H
#define PATRON_SERVICE_H

#include "PatronAccess.h"

#include <vector>

class Patron;
class CreditVerifier;
class Emailer;

class PatronService {
public:
  PatronService();

  explicit PatronService(const CreditVerifier *verifier, const Emailer *emailer = nullptr)
      : mVerifier{verifier}, mEmailer{emailer} {}

  virtual ~PatronService(void);

  void add(const std::string &name, const std::string &cardNumber);
  void add(const Patron &);
  bool find(Patron &) const;
  Patron findByCardNumber(const std::string &cardNumber) const;
  Patron findByClassification(const std::string &classification) const;
  short patronCount() const;
  void update(const Patron &);
  std::vector<Patron> getAll() const; // bad!
  static void deleteAll();

private:
    PatronAccess mPatronAccess;
    const CreditVerifier* mVerifier = nullptr;
    const Emailer* mEmailer = nullptr;
};

#endif
