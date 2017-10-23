#include "gmock/gmock.h"

#include "StockPortfolio.h"

using namespace testing;

TEST(AStockPortfolio, isEmptyOnCreation)
{
    StockPortfolio portfolio;

    ASSERT_TRUE(portfolio.empty());
}

TEST(AStockPortfolio, isNotEmptyAfterPurchase)
{
    StockPortfolio portfolio;
    portfolio.purchase("APPL", 3);

    ASSERT_FALSE(portfolio.empty());
}

TEST(AStockPortfolio, uniqueSymbolCountIsZeroOnCreation)
{
    StockPortfolio portfolio;

    ASSERT_THAT(portfolio.countUnique(), Eq(0));
}

TEST(AStockPortfolio, uniqueSymbolCountIncrementsAfterPurchasingNewSymbol)
{
    StockPortfolio portfolio;
    portfolio.purchase("APPL", 3);
    ASSERT_THAT(portfolio.countUnique(), Eq(1));
}

TEST(AStockPortfolio, uniqueSymbolCountOnlyIncrementsOnceAfterPurchasingSameSymbol)
{
    StockPortfolio portfolio;
    portfolio.purchase("APPL", 1);
    ASSERT_THAT(portfolio.countUnique(), Eq(1));
    portfolio.purchase("APPL", 1);
    ASSERT_THAT(portfolio.countUnique(), Eq(1));
}

TEST(AStockPortfolio, uniqueSymbolCountIncrementsOncePerNewSymbolPurchase)
{
    StockPortfolio portfolio;
    portfolio.purchase("APPL", 3);
    ASSERT_THAT(portfolio.countUnique(), Eq(1));
    portfolio.purchase("IBM", 5);
    ASSERT_THAT(portfolio.countUnique(), Eq(2));
    portfolio.purchase("GOOGL", 5);
    ASSERT_THAT(portfolio.countUnique(), Eq(3));
}

TEST(AStockPortfolio, hasNoSharesOnCreation)
{
    StockPortfolio portfolio;
    const char* stocks[] = {"APPL", "GOOGL", "IBM", "foo", "bar", "baz"};
    for (auto stock: stocks)
    {
        ASSERT_THAT(portfolio.count(stock), Eq(0));
    }
}

TEST(AStockPortfolio, purchasesExpectedAmountOfShares)
{
    StockPortfolio portfolio;
    portfolio.purchase("APPL", 3);
    ASSERT_THAT(portfolio.count("APPL"), Eq(3));
}

TEST(AStockPortfolio, countIncrementsOnceAfterSuccessivePurchases)
{
    StockPortfolio portfolio;
    portfolio.purchase("APPL", 1);
    ASSERT_THAT(portfolio.count("APPL"), Eq(1));
    portfolio.purchase("APPL", 1);
    ASSERT_THAT(portfolio.count("APPL"), Eq(2));
}


TEST(AStockPortfolio, countDecrementsWhenSellingShares)
{
    StockPortfolio portfolio;
    portfolio.purchase("APPL", 5);
    portfolio.sell("APPL", 3);
    ASSERT_THAT(portfolio.count("APPL"), Eq(2));
}

TEST(AStockPortfolio, countIsZeroAfterSellingAllShares)
{
    StockPortfolio portfolio;
    portfolio.purchase("APPL", 5);
    portfolio.sell("APPL", 5);
    ASSERT_THAT(portfolio.count("APPL"), Eq(0));
    ASSERT_THAT(portfolio.countUnique(), Eq(0));
    ASSERT_TRUE(portfolio.empty());
}

TEST(AStockPortfolio, cannotSellNonOwnedShares)
{
    StockPortfolio portfolio;
    portfolio.purchase("APPL", 3);
    ASSERT_THROW(portfolio.sell("APPL", 5), InvalidTradeException);
}
