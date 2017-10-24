#include "gmock/gmock.h"
#include "Holding.h"
#include "Book.h"
#include "ClassificationData.h"
#include "BranchTestData.h"
#include "Branch.h"
#include "InvalidBarcodeException.h"
#include "boost/date_time/gregorian/gregorian_types.hpp"

using namespace boost::gregorian;
using namespace BranchTestData;
using namespace ClassificationData;
using namespace std;
using namespace testing;

/*
This test class is a mess. Opportunities for cleanup might include:

 - AAA used but no visual separation
 - seeming use of AAA but it's not really
 - constant names that obscure relevant information
 - can data be created in the test?
 - poor / inconsistent test names
 - comments in tests (are they even true)?
 - multiple behaviors/asserts per test
 - code in the wrong place / opportunities for reuse of existing code
 - dead code
 */

namespace
{
    bool IsAvailableAt(const Holding& aHolding, const Branch& branch) {
        return aHolding.currentBranch() == branch &&
            aHolding.isAvailable();
    }

    void MakeAvailableAtABranch(Holding& holding) {
        holding.transfer(EAST_BRANCH);
    }
}

class HoldingTest : public Test
{
public:
    Holding holding_{THE_TRIAL_CLASSIFICATION, 1};
    static const date ARBITRARY_DATE;
};

const date HoldingTest::ARBITRARY_DATE(2013, Jan, 1);

TEST_F(HoldingTest, BarcodeRequiresColon) {
    ASSERT_THROW(Holding("A"), InvalidBarcodeException);
}

TEST_F(HoldingTest, ValidCreationDoesNotThrow) {
    ASSERT_NO_THROW(Holding("A123", 3));
    ASSERT_NO_THROW(Holding("A", 1));
}

TEST_F(HoldingTest, CanExtractClassificationWhenCreatedWithBarcode) {
    Holding holding(Holding::constructBarcode("A123", 3));

    ASSERT_THAT(holding.classification(), StrEq("A123"));
}

TEST_F(HoldingTest, CanExtractCopyNumberWhenCreatedWithBarcode) {
    Holding h(Holding::constructBarcode("A234", 5));

    ASSERT_THAT(h.copyNumber(), Eq(5));
}

TEST_F(HoldingTest, IsNotAvailableWhenCreated) {
    Holding holding(Holding::constructBarcode("A", 1));

    ASSERT_THAT(holding.isAvailable(), Eq(false));
}

TEST_F(HoldingTest, CanSpecifyClassificationSeparatelyWhenCreated) {
    Holding holding("B", 2);

    ASSERT_THAT(holding.classification(), StrEq("B"));
}

TEST_F(HoldingTest, CanSpecifyCopyNumberSeparatelyWhenCreated) {
    Holding holding("A", 7);

    ASSERT_THAT(holding.copyNumber(), Eq(7));
}

TEST_F(HoldingTest, IsNotAvailableWhenCreatedWithSeparateClassificationAndCopy) {
    Holding holding("A", 2);

    ASSERT_THAT(holding.isAvailable(), Eq(false));
}

/*
* assignment copy all members
*/
TEST_F(HoldingTest, AssignmentCopiesAllMembers) {
	// transfer to a different branch
    holding_.transfer(EAST_BRANCH);
    Holding newHolding = holding_;
    ASSERT_THAT(newHolding.classification(), Eq(THE_TRIAL_CLASSIFICATION));
    ASSERT_THAT(newHolding.copyNumber(), Eq(holding_.copyNumber()));
    ASSERT_THAT(IsAvailableAt(newHolding, EAST_BRANCH), Eq(true));
}

TEST_F(HoldingTest, TransferMakesHoldingAvailableAtBranch) {
    holding_.transfer(EAST_BRANCH);

    ASSERT_THAT(IsAvailableAt(holding_, EAST_BRANCH), Eq(true));
}

TEST_F(HoldingTest, BarCodeCombinesClassificationAndCopyNumber) {
    Holding holding(THE_TRIAL_CLASSIFICATION, 5);
    string barcode = holding.barcode();
    ASSERT_THAT(barcode, StrEq(THE_TRIAL_CLASSIFICATION + ":5"));
}

TEST_F(HoldingTest, AreEqualWhenClassificationAndCopyMatch) {
    Holding copy(holding_.classification(), holding_.copyNumber());

    ASSERT_THAT(holding_ == copy, Eq(true));
}

TEST_F(HoldingTest, AreUnequalWhenCopyDoesNotMatch) {
    Holding extraCopy(holding_.classification(), holding_.copyNumber() + 1);

    ASSERT_THAT(holding_ != extraCopy, Eq(true));
}

TEST_F(HoldingTest, AreUnequalWhenClassificationDoesNotMatch) {
    Holding differentBook(holding_.classification() + "X", 1);

    ASSERT_THAT(holding_ != differentBook, Eq(true));
}

TEST_F(HoldingTest, IsLessThanWhenClassificationsAreLessThan) {
    Holding a("A:1");
    Holding b("B:1");

    ASSERT_THAT(a < b, Eq(true));
}

TEST_F(HoldingTest, IsNotLessThanWhenClassificationIsNotLessThan) {
    Holding a("A:1");
    Holding b("B:1");

    ASSERT_THAT(!(b < a), Eq(true));
}

TEST_F(HoldingTest, IsNotLessThanWhenBarcodesAreEqual) {
    Holding a("A:1");
    Holding aCopy("A:1");

    ASSERT_THAT(a < aCopy, Eq(false));
}

TEST_F(HoldingTest, ck) {
    holding_.transfer(EAST_BRANCH);
    date ckon(2007, Mar, 1);
    holding_.checkOut(ckon);
    ASSERT_THAT(holding_.isAvailable(), Eq(false));
    ASSERT_THAT(holding_.lastCheckedOutOn(), Eq(ckon));
    // verify late
    date_duration daysCheckedOut(Book::BOOK_CHECKOUT_PERIOD + 0);
    date expectedDue = ckon + daysCheckedOut;
    ASSERT_THAT(holding_.dueDate(), Eq(expectedDue));
}

TEST_F(HoldingTest, Ckin) {
    holding_.transfer(EAST_BRANCH);
    date checkoutOn(2007, Mar, 1);
    holding_.checkOut(checkoutOn);
    date checkinOn(2007, Mar, 2);
    Branch branch2("2", "branch2");
    holding_.checkIn(checkinOn, branch2);
    ASSERT_THAT(IsAvailableAt(holding_, branch2), Eq(true));
}

TEST_F(HoldingTest, Due) {
    Holding holdingA(SEVEN_CLASSIFICATION, 1);
    holdingA.transfer(EAST_BRANCH);
    date checkoutOn(2007, Mar, 1);
    holdingA.checkOut(checkoutOn);

    ASSERT_THAT(holdingA.dueDate(), Eq(checkoutOn + date_duration(Book::MOVIE_CHECKOUT_PERIOD)));
}

TEST_F(HoldingTest, MoviesDueCheckoutPeriodDaysAfterCheckout) {
    Holding movie(SEVEN_CLASSIFICATION, 1);
    MakeAvailableAtABranch(movie);
    movie.checkOut(ARBITRARY_DATE);
    ASSERT_THAT(movie.dueDate(),
        Eq(ARBITRARY_DATE + date_duration(Book::MOVIE_CHECKOUT_PERIOD)));
}
