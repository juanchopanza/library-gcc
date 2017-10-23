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
    portfolio.purchase("AAPL", 3);
    ASSERT_FALSE(portfolio.empty());
}

TEST_F(AStockPortfolio, uniqueSymbolCountIsZeroOnCreation)
{
    ASSERT_THAT(portfolio.countUnique(), Eq(0));
}

TEST_F(AStockPortfolio, uniqueSymbolCountIncrementsAfterPurchasingNewSymbol)
{
    portfolio.purchase("AAPL", 3);
    ASSERT_THAT(portfolio.countUnique(), Eq(1));
}

TEST_F(AStockPortfolio, uniqueSymbolCountOnlyIncrementsOnceAfterPurchasingSameSymbol)
{
    portfolio.purchase("AAPL", 1);
    ASSERT_THAT(portfolio.countUnique(), Eq(1));
    portfolio.purchase("AAPL", 1);
    ASSERT_THAT(portfolio.countUnique(), Eq(1));
}

TEST_F(AStockPortfolio, uniqueSymbolCountIncrementsOncePerNewSymbolPurchase)
{
    portfolio.purchase("AAPL", 3);
    ASSERT_THAT(portfolio.countUnique(), Eq(1));
    portfolio.purchase("IBM", 5);
    ASSERT_THAT(portfolio.countUnique(), Eq(2));
    portfolio.purchase("GOOGL", 5);
    ASSERT_THAT(portfolio.countUnique(), Eq(3));
}

TEST_F(AStockPortfolio, hasNoSharesOnCreation)
{
    const char* stocks[] = {"AAPL", "GOOGL", "IBM", "foo", "bar", "baz"};
    for (auto stock: stocks)
    {
        ASSERT_THAT(portfolio.count(stock), Eq(0));
    }
}

TEST_F(AStockPortfolio, purchasesExpectedAmountOfShares)
{
    portfolio.purchase("AAPL", 3);
    ASSERT_THAT(portfolio.count("AAPL"), Eq(3));
}

TEST_F(AStockPortfolio, countIncrementsOnceAfterSuccessivePurchases)
{
    portfolio.purchase("AAPL", 1);
    ASSERT_THAT(portfolio.count("AAPL"), Eq(1));
    portfolio.purchase("AAPL", 1);
    ASSERT_THAT(portfolio.count("AAPL"), Eq(2));
}


TEST_F(AStockPortfolio, countDecrementsWhenSellingShares)
{
    portfolio.purchase("AAPL", 5);
    portfolio.sell("AAPL", 3);
    ASSERT_THAT(portfolio.count("AAPL"), Eq(2));
}

TEST_F(AStockPortfolio, countIsZeroAfterSellingAllShares)
{
    portfolio.purchase("AAPL", 5);
    portfolio.sell("AAPL", 5);
    ASSERT_THAT(portfolio.count("AAPL"), Eq(0));
    ASSERT_THAT(portfolio.countUnique(), Eq(0));
    ASSERT_TRUE(portfolio.empty());
}

TEST_F(AStockPortfolio, cannotSellNonOwnedShares)
{
    portfolio.purchase("AAPL", 3);
    ASSERT_THROW(portfolio.sell("AAPL", 5), InvalidTradeException);
}

TEST_F(AStockPortfolio, cannotSellFromEmptyPortfolio)
{
    ASSERT_THROW(portfolio.sell("AAPL", 5), InvalidTradeException);
}
