#include "gmock/gmock.h"

#include "StockPortfolio.h"

using namespace testing;

struct AStockPortfolio : Test
{
    StockPortfolio portfolio;
};

TEST_F(AStockPortfolio, isEmptyOnCreation)
{
    ASSERT_TRUE(portfolio.empty());
}

TEST_F(AStockPortfolio, isNotEmptyAfterPurchase)
{
    portfolio.purchase("APPL", 3);
    ASSERT_FALSE(portfolio.empty());
}

TEST_F(AStockPortfolio, uniqueSymbolCountIsZeroOnCreation)
{
    ASSERT_THAT(portfolio.countUnique(), Eq(0));
}

TEST_F(AStockPortfolio, uniqueSymbolCountIncrementsAfterPurchasingNewSymbol)
{
    portfolio.purchase("APPL", 3);
    ASSERT_THAT(portfolio.countUnique(), Eq(1));
}

TEST_F(AStockPortfolio, uniqueSymbolCountOnlyIncrementsOnceAfterPurchasingSameSymbol)
{
    portfolio.purchase("APPL", 1);
    ASSERT_THAT(portfolio.countUnique(), Eq(1));
    portfolio.purchase("APPL", 1);
    ASSERT_THAT(portfolio.countUnique(), Eq(1));
}

TEST_F(AStockPortfolio, uniqueSymbolCountIncrementsOncePerNewSymbolPurchase)
{
    portfolio.purchase("APPL", 3);
    ASSERT_THAT(portfolio.countUnique(), Eq(1));
    portfolio.purchase("IBM", 5);
    ASSERT_THAT(portfolio.countUnique(), Eq(2));
    portfolio.purchase("GOOGL", 5);
    ASSERT_THAT(portfolio.countUnique(), Eq(3));
}

TEST_F(AStockPortfolio, hasNoSharesOnCreation)
{
    const char* stocks[] = {"APPL", "GOOGL", "IBM", "foo", "bar", "baz"};
    for (auto stock: stocks)
    {
        ASSERT_THAT(portfolio.count(stock), Eq(0));
    }
}

TEST_F(AStockPortfolio, purchasesExpectedAmountOfShares)
{
    portfolio.purchase("APPL", 3);
    ASSERT_THAT(portfolio.count("APPL"), Eq(3));
}

TEST_F(AStockPortfolio, countIncrementsOnceAfterSuccessivePurchases)
{
    portfolio.purchase("APPL", 1);
    ASSERT_THAT(portfolio.count("APPL"), Eq(1));
    portfolio.purchase("APPL", 1);
    ASSERT_THAT(portfolio.count("APPL"), Eq(2));
}


TEST_F(AStockPortfolio, countDecrementsWhenSellingShares)
{
    portfolio.purchase("APPL", 5);
    portfolio.sell("APPL", 3);
    ASSERT_THAT(portfolio.count("APPL"), Eq(2));
}

TEST_F(AStockPortfolio, countIsZeroAfterSellingAllShares)
{
    portfolio.purchase("APPL", 5);
    portfolio.sell("APPL", 5);
    ASSERT_THAT(portfolio.count("APPL"), Eq(0));
    ASSERT_THAT(portfolio.countUnique(), Eq(0));
    ASSERT_TRUE(portfolio.empty());
}

TEST_F(AStockPortfolio, cannotSellNonOwnedShares)
{
    portfolio.purchase("APPL", 3);
    ASSERT_THROW(portfolio.sell("APPL", 5), InvalidTradeException);
}

TEST_F(AStockPortfolio, cannotSellFromEmptyPortfolio)
{
    ASSERT_THROW(portfolio.sell("APPL", 5), InvalidTradeException);
}
