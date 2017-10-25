#include "PatronService.h"
#include "Patron.h"
#include "Holding.h"
#include "ClassificationData.h"
#include "CreditVerifier.h"
#include "Emailer.h"

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
        APatronService::TearDown();
        delete jane;
        delete joe;
    }
};

struct CreditVerifierMock : CreditVerifier{
    MOCK_CONST_METHOD1(hasCredit, bool(const std::string&));
};

struct EmailerMock : Emailer
{
    MOCK_CONST_METHOD4(send, void(const string&, const string&, const string&, const string&));
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
    CreditVerifierMock verifier;

    PatronService service(&verifier);
    Patron joe("Joe", "p1");
    EXPECT_CALL(verifier, hasCredit(joe.creditCardNumber())).WillOnce(Return(false));
    service.add(joe);
    ASSERT_THAT(service.patronCount(), Eq(0));
}

TEST_F(APatronService, AddSucceedsWhenCreditSufficient) {
    CreditVerifierMock verifier;
    PatronService service(&verifier);
    Patron joe("Joe", "p1");
    EXPECT_CALL(verifier, hasCredit(joe.creditCardNumber())).WillOnce(Return(true));
    service.add(joe);
    ASSERT_THAT(service.patronCount(), Eq(1));
}

TEST_F(APatronService, AddFailureSendsEmail) {
    CreditVerifierMock verifier;
    EmailerMock emailer;

	PatronService service(&verifier, &emailer);
	Patron joe("Joe", "p1");
	EXPECT_CALL(verifier, hasCredit(joe.creditCardNumber())).WillOnce(Return(false));
	EXPECT_CALL(emailer, send(_, _, _, _)).WillOnce(Return());
	service.add(joe);
	ASSERT_THAT(service.patronCount(), Eq(0));
}

TEST_F(APatronService, verificationUsesCreditCardNumber)
{
    CreditVerifierMock verifier;

    PatronService service(&verifier);
    Patron joe("Joe", "p1", "LOW_CREDIT");
    Patron jane("Jane", "p11", "OK_CREDIT");
    EXPECT_CALL(verifier, hasCredit(joe.creditCardNumber())).WillOnce(Return(false));
    EXPECT_CALL(verifier, hasCredit(jane.creditCardNumber())).WillOnce(Return(true));

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
