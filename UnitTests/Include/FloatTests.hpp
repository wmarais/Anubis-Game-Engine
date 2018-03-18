#ifndef ANUBIS_UNIT_TEST_FLOAT_TEST_HPP
#define ANUBIS_UNIT_TEST_FLOAT_TEST_HPP

#include <gtest/gtest.h>
#include "../../Include/Anubis/Common/Float.hpp"

using namespace std;
using namespace Anubis;

/*##############################################################################
 * FLOATING POINT COMPARISON TESTS
 * -------------------------------
 * Compare floating point numbers using the Float::compare function. The tests
 * are written with consideration to the Anubis Game libraries and as such
 * the error margin "Epsilon" is left at it's default value.
 *############################################################################*/
/***************************************************************************//**
 * Float::compare general large numbers.
 ******************************************************************************/
TEST(float, ComparePosBigNumbers)
{
  EXPECT_TRUE(Float::compare(1000000.0f, 1000001.0f));
  EXPECT_TRUE(Float::compare(1000001.0f, 1000000.0f));
  EXPECT_FALSE(Float::compare(10000.0f, 10001.0f));
  EXPECT_FALSE(Float::compare(10001.0f, 10000.0f));
}

/***************************************************************************//**
 * Float::compare negative large numbers.
 ******************************************************************************/
TEST(float, CcompareNegBigNumbers)
{
  EXPECT_TRUE(Float::compare(-1000000.0f, -1000001.0f));
  EXPECT_TRUE(Float::compare(-1000001.0f, -1000000.0f));
  EXPECT_FALSE(Float::compare(-10000.0f, -10001.0f));
  EXPECT_FALSE(Float::compare(-10001.0f, -10000.0f));
}

/***************************************************************************//**
 * Float::compare numbers around 1.0f.
 ******************************************************************************/
TEST(float, ComparePosMidNumbers)
{
  EXPECT_TRUE(Float::compare(1.0000001f, 1.0000002f));
  EXPECT_TRUE(Float::compare(1.0000002f, 1.0000001f));
  EXPECT_FALSE(Float::compare(1.0002f, 1.0001f));
  EXPECT_FALSE(Float::compare(1.0001f, 1.0002f));
}

/***************************************************************************//**
 * Float::compare numbers around -1.0f.
 ******************************************************************************/
TEST(float, CompareNegMidNumbers)
{
  EXPECT_TRUE(Float::compare(-1.000001f, -1.000002f));
  EXPECT_TRUE(Float::compare(-1.000002f, -1.000001f));
  EXPECT_FALSE(Float::compare(-1.0001f, -1.0002f));
  EXPECT_FALSE(Float::compare(-1.0002f, -1.0001f));
}

/***************************************************************************//**
 * Float::compare numbers between 1.0f and 0.0f.
 ******************************************************************************/
TEST(float, ComparePosSmallNumbers)
{
  EXPECT_TRUE(Float::compare(0.000000001000001f, 0.000000001000002f));
  EXPECT_TRUE(Float::compare(0.000000001000002f, 0.000000001000001f));
  EXPECT_FALSE(Float::compare(0.000000000001002f, 0.000000000001001f));
  EXPECT_FALSE(Float::compare(0.000000000001001f, 0.000000000001002f));
}

/***************************************************************************//**
 * Float::compare numbers between -1.0f and 0.0f.
 ******************************************************************************/
TEST(float, CompareNegSmallNumbers)
{
  EXPECT_TRUE(Float::compare(-0.000000001000001f, -0.000000001000002f));
  EXPECT_TRUE(Float::compare(-0.000000001000002f, -0.000000001000001f));
  EXPECT_FALSE(Float::compare(-0.000000000001002f, -0.000000000001001f));
  EXPECT_FALSE(Float::compare(-0.000000000001001f, -0.000000000001002f));
}

/***************************************************************************//**
 * Float::compare small diffirences away from zero.
 ******************************************************************************/
TEST(float, CompareSmallDiffirences)
{
  EXPECT_TRUE(Float::compare(0.3f, 0.30000003f));
  EXPECT_TRUE(Float::compare(-0.3f, -0.30000003f));
}

/***************************************************************************//**
 * Float::compare numbers involving 0.0f.
 ******************************************************************************/
TEST(float, CompareNearZeroNumbers)
{
  EXPECT_TRUE(Float::compare(0.0f, 0.0f));
  EXPECT_TRUE(Float::compare(0.0f, -0.0f));
  EXPECT_TRUE(Float::compare(-0.0f, -0.0f));
  EXPECT_FALSE(Float::compare(0.00000001f, 0.0f));
  EXPECT_FALSE(Float::compare(0.0f, 0.00000001f));
  EXPECT_FALSE(Float::compare(-0.00000001f, 0.0f));
  EXPECT_FALSE(Float::compare(0.0f, -0.00000001f));

  EXPECT_TRUE(Float::compare(0.0f, 1e-40f, 0.01f));
  EXPECT_TRUE(Float::compare(1e-40f, 0.0f, 0.01f));
  EXPECT_FALSE(Float::compare(1e-40f, 0.0f, 0.000001f));
  EXPECT_FALSE(Float::compare(0.0f, 1e-40f, 0.000001f));

  EXPECT_TRUE(Float::compare(0.0f, -1e-40f, 0.1f));
  EXPECT_TRUE(Float::compare(-1e-40f, 0.0f, 0.1f));
  EXPECT_FALSE(Float::compare(-1e-40f, 0.0f, 0.00000001f));
  EXPECT_FALSE(Float::compare(0.0f, -1e-40f, 0.00000001f));
}

/***************************************************************************//**
 * Float::compare extreme number values.
 ******************************************************************************/
TEST(float, CompareExtremeNumbers)
{
  EXPECT_TRUE(Float::compare(numeric_limits<float>::max(),
                              numeric_limits<float>::max()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::max(),
                               -numeric_limits<float>::max()));
  EXPECT_FALSE(Float::compare(-numeric_limits<float>::max(),
                               numeric_limits<float>::max()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::max(),
                               numeric_limits<float>::max() / 2.0f));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::max(),
                               -numeric_limits<float>::max() / 2.0f));
  EXPECT_FALSE(Float::compare(-numeric_limits<float>::max(),
                               numeric_limits<float>::max() / 2.0f));
}

/***************************************************************************//**
 * Test infinity to infinity and infinity to max value comparisons.
 ******************************************************************************/
TEST(float, CompareInfinities)
{
  EXPECT_TRUE(Float::compare(numeric_limits<float>::infinity(),
                              numeric_limits<float>::infinity()));
  EXPECT_TRUE(Float::compare(-numeric_limits<float>::infinity(),
                              -numeric_limits<float>::infinity()));
  EXPECT_FALSE(Float::compare(-numeric_limits<float>::infinity(),
                               numeric_limits<float>::infinity()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::infinity(),
                               numeric_limits<float>::max()));
  EXPECT_FALSE(Float::compare(-numeric_limits<float>::infinity(),
                               -numeric_limits<float>::max()));
}

/***************************************************************************//**
 * Test NaN to inifinity, NaN to NaN, NaN to Max, NaN to Min and NaN to +/-0.
 ******************************************************************************/
TEST(float, CompareNaN)
{
  EXPECT_FALSE(Float::compare(numeric_limits<float>::quiet_NaN(),
                               numeric_limits<float>::quiet_NaN()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::quiet_NaN(), 0.0f));
  EXPECT_FALSE(Float::compare(-0.0f, numeric_limits<float>::quiet_NaN()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::quiet_NaN(), -0.0f));
  EXPECT_FALSE(Float::compare(0.0f, numeric_limits<float>::quiet_NaN()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::quiet_NaN(),
                               numeric_limits<float>::infinity()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::infinity(),
                               numeric_limits<float>::quiet_NaN()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::quiet_NaN(),
                               -numeric_limits<float>::infinity()));
  EXPECT_FALSE(Float::compare(-numeric_limits<float>::infinity(),
                               numeric_limits<float>::quiet_NaN()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::quiet_NaN(),
                               numeric_limits<float>::max()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::max(),
                               numeric_limits<float>::quiet_NaN()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::quiet_NaN(),
                               -numeric_limits<float>::max()));
  EXPECT_FALSE(Float::compare(-numeric_limits<float>::max(),
                               numeric_limits<float>::quiet_NaN()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::quiet_NaN(),
                               numeric_limits<float>::denorm_min()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::denorm_min(),
                               numeric_limits<float>::quiet_NaN()));
  EXPECT_FALSE(Float::compare(numeric_limits<float>::quiet_NaN(),
                               -numeric_limits<float>::denorm_min()));
  EXPECT_FALSE(Float::compare(-numeric_limits<float>::denorm_min(),
                               numeric_limits<float>::quiet_NaN()));
}
/***************************************************************************//**
 * Test comparison of numbers on both sides of 0.
 ******************************************************************************/
TEST(float, CompareOpposite)
{
  EXPECT_FALSE(Float::compare(1.000000001f, -1.0f));
  EXPECT_FALSE(Float::compare(-1.0f, 1.000000001f));
  EXPECT_FALSE(Float::compare(-1.000000001f, 1.0f));
  EXPECT_FALSE(Float::compare(1.0f, -1.000000001f));
  EXPECT_TRUE(Float::compare(10 * std::numeric_limits<float>::denorm_min(),
                      10 * -std::numeric_limits<float>::denorm_min()));

  EXPECT_FALSE(Float::compare(10000 * std::numeric_limits<float>::denorm_min(),
                       10000 * -std::numeric_limits<float>::denorm_min()));
}

/***************************************************************************//**
 * Test comparison of numbers very close to zero.
 ******************************************************************************/
TEST(float, CompareNearZero)
{
  EXPECT_TRUE(Float::compare(std::numeric_limits<float>::denorm_min(),
                      std::numeric_limits<float>::denorm_min()));
  EXPECT_TRUE(Float::compare(std::numeric_limits<float>::denorm_min(),
                      -std::numeric_limits<float>::denorm_min()));
  EXPECT_TRUE(Float::compare(-std::numeric_limits<float>::denorm_min(),
                      std::numeric_limits<float>::denorm_min()));
  EXPECT_TRUE(Float::compare(std::numeric_limits<float>::denorm_min(), 0.0f));
  EXPECT_TRUE(Float::compare(0.0f, std::numeric_limits<float>::denorm_min()));
  EXPECT_TRUE(Float::compare(-std::numeric_limits<float>::denorm_min(), 0.0f));
  EXPECT_TRUE(Float::compare(0.0f, -std::numeric_limits<float>::denorm_min()));

  EXPECT_FALSE(Float::compare(0.000000001f,
                       -std::numeric_limits<float>::denorm_min()));
  EXPECT_FALSE(Float::compare(0.000000001f,
                       std::numeric_limits<float>::denorm_min()));
  EXPECT_FALSE(Float::compare(std::numeric_limits<float>::denorm_min(),
                       0.000000001f));
  EXPECT_FALSE(Float::compare(-std::numeric_limits<float>::denorm_min(),
                       0.000000001f));
}

#endif /* ANUBIS_UNIT_TEST_FLOAT_TEST_HPP */
