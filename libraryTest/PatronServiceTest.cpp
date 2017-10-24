#include "PatronService.h"
#include "Patron.h"
#include "Holding.h"
#include "ClassificationData.h"
#include "CreditVerifier.h"

#include "gmock/gmock.h"

using namespace ClassificationData;
using namespace testing;
using namespace std;

struct APatronService : Test
{
    void TearDown() override {
        PatronService::deleteAll();
    }
};

class PatronServiceTest : public APatronService {
public:
    static const string CARD_NUMBER;
    PatronService service;
    Patron* joe;
    Patron* jane;

    virtual void SetUp() {
        PatronService::deleteAll();
        joe = new Patron("Joe", "p1");
        jane = new Patron("Jane", "p2");
    }

    virtual void TearDown() {
        delete jane;
        delete joe;
    }
};

TEST_F(PatronServiceTest, CountInitiallyZero) {
    ASSERT_THAT(service.patronCount(), Eq(0));
}

TEST_F(PatronServiceTest, AddUsingAttributes) {
    service.add("Suresh", "p20");

    Patron retrieved("", "p20");
    service.find(retrieved);
    ASSERT_THAT(retrieved.name(), StrEq("Suresh"));
}

TEST_F(PatronServiceTest, AddIncrementsCount) {
    service.add(*joe);
    ASSERT_THAT(service.patronCount(), Eq(1));

    service.add(*jane);
    ASSERT_THAT(service.patronCount(), Eq(2));
}

TEST_F(APatronService, AddFailsWhenCreditLow) {
    struct FailVerifier : CreditVerifier
    {
        unsigned int creditScore(const std::string& cardNumber) const override
        {
            // PatronService cuts as 650
            return 649;
        }
    } verifier;

    PatronService service(&verifier);
    Patron joe("Joe", "p1");
    service.add(joe);
    ASSERT_THAT(service.patronCount(), Eq(0));
}

TEST_F(APatronService, AddSucceedsWhenCreditSufficient) {
    struct SuccessVerifier : CreditVerifier
    {
        unsigned int creditScore(const std::string& cardNumber) const override
        {
            // PatronService cuts as 650
            return 650;
        }
    } verifier;

    PatronService service(&verifier);
    Patron joe("Joe", "p1");
    service.add(joe);
    ASSERT_THAT(service.patronCount(), Eq(1));
}

TEST_F(APatronService, verificationUsesCreditCardNumber) {

    struct SuccessVerifier : CreditVerifier
    {
        unsigned int creditScore(const std::string& cardNumber) const override
        {
            // PatronService cuts as 650
            const static std::map<std::string, unsigned int> CREDIT_MAP = {
                {"LOW_CREDIT", 649},
                {"OK_CREDIT", 650}
            };
            return CREDIT_MAP.at(cardNumber);
        }
    } verifier;

    PatronService service(&verifier);
    Patron joe("Joe", "p1", "LOW_CREDIT");
    Patron jane("Jane", "p11", "OK_CREDIT");
    service.add(joe);
    service.add(jane);
    auto patrons = service.getAll();
    ASSERT_THAT(patrons, ElementsAre(jane));
}

TEST_F(PatronServiceTest, DeleteAllSetsCountToZero) {
    service.add(*joe);
    service.add(*jane);

    PatronService::deleteAll();

    ASSERT_THAT(service.patronCount(), Eq(0));
}

TEST_F(PatronServiceTest, DeleteAllRemovesAnyAddedPatrons) {
    service.add(*joe);
    service.add(*jane);

    PatronService::deleteAll();

    ASSERT_THAT(service.patronCount(), Eq(0));
}

TEST_F(PatronServiceTest, FindAnswerFalseForNonexistentPatron) {
    ASSERT_THAT(service.find(*joe), Eq(false));
}

TEST_F(PatronServiceTest, FindAnswersTrueForAddedPatron) {
    service.add(*joe);

    bool found = service.find(*joe);

    ASSERT_THAT(found, Eq(true));
}

TEST_F(PatronServiceTest, FindRetrieves) {
    service.add(*joe);
    Patron retrieved("dummy name", joe->cardNumber());

    service.find(retrieved);

    ASSERT_THAT(retrieved.name(), StrEq(joe->name()));
}

TEST_F(PatronServiceTest, RetrievesPatronByCardNumber) {
    service.add(*joe);

    Patron retrieved = service.findByCardNumber(joe->cardNumber());

    ASSERT_THAT(retrieved.name(), StrEq(joe->name()));
}

TEST_F(PatronServiceTest, MembersFullyPopulatedInFoundPatron) {
    joe->addFine(20);
    Holding theTrial(THE_TRIAL_CLASSIFICATION, 1);
    joe->borrow(theTrial);
    service.add(*joe);
    Patron retrieved("", joe->cardNumber());

    service.find(retrieved);

    ASSERT_THAT(retrieved.fineBalance(), Eq(20));
    ASSERT_THAT(retrieved.holdings(), Eq(set<Holding>{ theTrial }));
}

TEST_F(PatronServiceTest, RetrievesPatronByClassification) {
    Holding theTrial(THE_TRIAL_CLASSIFICATION, 1);
    joe->borrow(theTrial);
    service.add(*joe);

    Patron retrieved = service.findByClassification(THE_TRIAL_CLASSIFICATION);

    ASSERT_THAT(retrieved.name(), StrEq(joe->name()));
}

TEST_F(PatronServiceTest, RetrievesDefaultPatronWhenNotFoundByClassification) {
    service.add(*joe);

    Patron retrieved = service.findByClassification(THE_TRIAL_CLASSIFICATION);

    ASSERT_THAT(retrieved.name(), StrEq(""));
}
