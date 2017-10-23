#include <gmock/gmock.h>

#include "RomanNumerals.h"

using namespace testing;

TEST(RomanNumeralConverter, OneIsI)
{
    ASSERT_THAT(romanize(1), StrEq("I"));
}

TEST(RomanNumeralConverter, TwoIsII)
{
    ASSERT_THAT(romanize(2), StrEq("II"));
}

TEST(RomanNumeralConverter, ThreeIsIII)
{
    ASSERT_THAT(romanize(3), StrEq("III"));
}

TEST(RomanNumeralConverter, FourIsIV)
{
    ASSERT_THAT(romanize(4), StrEq("IV"));
}

TEST(RomanNumeralConverter, FiveIsV)
{
    ASSERT_THAT(romanize(5), StrEq("V"));
}

TEST(RomanNumeralConverter, SixIsVI)
{
    ASSERT_THAT(romanize(6), StrEq("VI"));
}

TEST(RomanNumeralConverter, SevenIsVII)
{
    ASSERT_THAT(romanize(7), StrEq("VII"));
}

TEST(RomanNumeralConverter, EightIsVIII)
{
    ASSERT_THAT(romanize(8), StrEq("VIII"));
}

