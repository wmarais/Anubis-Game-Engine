#ifndef ANUBIS_UNIT_TESTS_VECTOR4F_TESTS_HPP
#define ANUBIS_UNIT_TESTS_VECTOR4F_TESTS_HPP

#include <gtest/gtest.h>
#include "../../Include/Anubis/Math/Vector4f.hpp"

using namespace Anubis::Math;

/*##############################################################################
 * VECTOR4F TESTS
 * --------------
 *
 *############################################################################*/
/***************************************************************************//**
 * Test the comparison operator is working as expected.
 ******************************************************************************/
TEST(Vector4f, ComparisonOperator)
{
  /* Test a direction vector. */
  EXPECT_TRUE(Vector4f(1.0f, 2.0f, 3.0f, 0.0f) ==
              Vector4f(1.0f, 2.0f, 3.0f, 0.0f));

  /* Test a position vector. */
  EXPECT_TRUE(Vector4f(1.0f, 2.0f, 3.0f, 1.0f) ==
              Vector4f(1.0f, 2.0f, 3.0f, 1.0f));

  /* Test X component. */
  EXPECT_FALSE(Vector4f(0.0f, 2.0f, 3.0f, 0.0f) ==
               Vector4f(1.0f, 2.0f, 3.0f, 0.0f));

  EXPECT_FALSE(Vector4f(1.0f, 2.0f, 3.0f, 0.0f) ==
               Vector4f(0.0f, 2.0f, 3.0f, 0.0f));

  /* Test Y component. */
  EXPECT_FALSE(Vector4f(1.0f, 0.0f, 3.0f, 0.0f) ==
               Vector4f(1.0f, 2.0f, 3.0f, 0.0f));

  EXPECT_FALSE(Vector4f(1.0f, 2.0f, 3.0f, 0.0f) ==
               Vector4f(1.0f, 0.0f, 3.0f, 0.0f));

  /* Test Z component. */
  EXPECT_FALSE(Vector4f(1.0f, 2.0f, 0.0f, 0.0f) ==
               Vector4f(1.0f, 2.0f, 3.0f, 0.0f));

  EXPECT_FALSE(Vector4f(1.0f, 2.0f, 3.0f, 0.0f) ==
               Vector4f(1.0f, 2.0f, 0.0f, 0.0f));

  /* Test W component. */
  EXPECT_FALSE(Vector4f(1.0f, 2.0f, 3.0f, 1.0f) ==
               Vector4f(1.0f, 2.0f, 3.0f, 0.0f));

  EXPECT_FALSE(Vector4f(1.0f, 2.0f, 3.0f, 0.0f) ==
               Vector4f(1.0f, 2.0f, 3.0f, 1.0f));
}

/***************************************************************************//**
 * Test vector addition.
 ******************************************************************************/
TEST(Vector4f, AdditionOperator)
{
  /* Test Addition with diffirent combinations of the W component. */
  EXPECT_TRUE(Vector4f(1.0f, 2.0f, 3.0f, 0.0f) +
              Vector4f(4.0f, 5.0f, 6.0f, 0.0f) ==
              Vector4f(5.0f, 7.0f, 9.0f, 0.0f));

  EXPECT_TRUE(Vector4f(1.0f, 2.0f, 3.0f, 1.0f) +
              Vector4f(4.0f, 5.0f, 6.0f, 0.0f) ==
              Vector4f(5.0f, 7.0f, 9.0f, 1.0f));

  EXPECT_TRUE(Vector4f(1.0f, 2.0f, 3.0f, 0.0f) +
              Vector4f(4.0f, 5.0f, 6.0f, 1.0f) ==
              Vector4f(5.0f, 7.0f, 9.0f, 0.0f));
}

TEST(Vector4f, AddAndAssignOperator)
{
  /* Test Addition with diffirent combinations of the W component. */
  EXPECT_TRUE((Vector4f(1.0f, 2.0f, 3.0f, 0.0f) +=
              Vector4f(4.0f, 5.0f, 6.0f, 0.0f)) ==
              Vector4f(5.0f, 7.0f, 9.0f, 0.0f));

  EXPECT_TRUE((Vector4f(1.0f, 2.0f, 3.0f, 1.0f) +=
              Vector4f(4.0f, 5.0f, 6.0f, 0.0f)) ==
              Vector4f(5.0f, 7.0f, 9.0f, 1.0f));

  EXPECT_TRUE((Vector4f(1.0f, 2.0f, 3.0f, 0.0f) +=
              Vector4f(4.0f, 5.0f, 6.0f, 1.0f)) ==
              Vector4f(5.0f, 7.0f, 9.0f, 0.0f));
}

TEST(Vector4f, SubtractionOperator)
{
  /* Test Subtraction with diffirent combinations of the W component. */
  EXPECT_TRUE(Vector4f(1.0f, 2.0f, 3.0f, 0.0f) -
              Vector4f(4.0f, 6.0f, 5.0f, 0.0f) ==
              Vector4f(-3.0f, -4.0f, -2.0f, 0.0f));

  EXPECT_TRUE(Vector4f(1.0f, 2.0f, 3.0f, 1.0f) -
              Vector4f(4.0f, 6.0f, 5.0f, 0.0f) ==
              Vector4f(-3.0f, -4.0f, -2.0f, 1.0f));

  EXPECT_TRUE(Vector4f(1.0f, 2.0f, 3.0f, 0.0f) -
              Vector4f(4.0f, 6.0f, 5.0f, 1.0f) ==
              Vector4f(-3.0f, -4.0f, -2.0f, 0.0f));
}

TEST(Vector4f, SubtractAndAssignOperator)
{
  /* Test Subtraction with diffirent combinations of the W component. */
  EXPECT_TRUE((Vector4f(1.0f, 2.0f, 3.0f, 0.0f) -=
              Vector4f(4.0f, 6.0f, 5.0f, 0.0f)) ==
              Vector4f(-3.0f, -4.0f, -2.0f, 0.0f));

  EXPECT_TRUE((Vector4f(1.0f, 2.0f, 3.0f, 1.0f) -=
              Vector4f(4.0f, 6.0f, 5.0f, 0.0f)) ==
              Vector4f(-3.0f, -4.0f, -2.0f, 1.0f));

  EXPECT_TRUE((Vector4f(1.0f, 2.0f, 3.0f, 0.0f) -=
              Vector4f(4.0f, 6.0f, 5.0f, 1.0f)) ==
              Vector4f(-3.0f, -4.0f, -2.0f, 0.0f));
}

TEST(Vector4f, MultiplicationOperator)
{
  /* Test Multiplication with diffirent combinations of the W component. */
  EXPECT_TRUE(Vector4f(1.0f, 2.0f, 3.0f, 0.0f) * 2.0f ==
              Vector4f(2.0f, 4.0f, 6.0f, 0.0f));

  EXPECT_TRUE(Vector4f(1.0f, 2.0f, 3.0f, 1.0f) * 2.0f ==
              Vector4f(2.0f, 4.0f, 6.0f, 1.0f));
}

TEST(Vector4f, MultiplyAndAssignOperator)
{
  /* Test Multiplication with diffirent combinations of the W component. */
  EXPECT_TRUE((Vector4f(1.0f, 2.0f, 3.0f, 0.0f) *= 2.0f) ==
              Vector4f(2.0f, 4.0f, 6.0f, 0.0f));

  EXPECT_TRUE((Vector4f(1.0f, 2.0f, 3.0f, 1.0f) *= 2.0f) ==
              Vector4f(2.0f, 4.0f, 6.0f, 1.0f));
}

TEST(Vector4f, DivisionOperator)
{
  /* Test Division with diffirent combinations of the W component. */
  EXPECT_TRUE(Vector4f(2.0f, 4.0f, 6.0f, 0.0f) / 2.0f ==
              Vector4f(1.0f, 2.0f, 3.0f, 0.0f));

  EXPECT_TRUE(Vector4f(2.0f, 4.0f, 6.0f, 1.0f) / 2.0f ==
              Vector4f(1.0f, 2.0f, 3.0f, 1.0f));
}

TEST(Vector4f, DivideAndAssignOperator)
{
  /* Test Division with diffirent combinations of the W component. */
  EXPECT_TRUE((Vector4f(2.0f, 4.0f, 6.0f, 0.0f) /= 2.0f) ==
              Vector4f(1.0f, 2.0f, 3.0f, 0.0f));

  EXPECT_TRUE((Vector4f(2.0f, 4.0f, 6.0f, 1.0f) /= 2.0f) ==
              Vector4f(1.0f, 2.0f, 3.0f, 1.0f));
}

/***************************************************************************//**
 * Test the cross product calculations and confirm that the W component does not
 * affect the result.
 ******************************************************************************/
TEST(Vector4f, CrossProduct)
{
  /* Calculate that Z axis using cross(X, Y). */
  EXPECT_EQ(kZAxis4f, Vector4f(1.0f, 0.0f, 0.0f, 0.0f).cross(
                      Vector4f(0.0f, 1.0f, 0.0f, 0.0f)));

  EXPECT_EQ(kZAxis4f, Vector4f(1.0f, 0.0f, 0.0f, 1.0f).cross(
                      Vector4f(0.0f, 1.0f, 0.0f, 0.0f)));

  EXPECT_EQ(kZAxis4f, Vector4f(1.0f, 0.0f, 0.0f, 0.0f).cross(
                      Vector4f(0.0f, 1.0f, 0.0f, 1.0f)));

  /* Calculate that X axis using cross(Y, Z). */
  EXPECT_EQ(kXAxis4f, Vector4f(0.0f, 1.0f, 0.0f, 0.0f).cross(
                      Vector4f(0.0f, 0.0f, 1.0f, 0.0f)));

  EXPECT_EQ(kXAxis4f, Vector4f(0.0f, 1.0f, 0.0f, 1.0f).cross(
                      Vector4f(0.0f, 0.0f, 1.0f, 0.0f)));

  EXPECT_EQ(kXAxis4f, Vector4f(0.0f, 1.0f, 0.0f, 0.0f).cross(
                      Vector4f(0.0f, 0.0f, 1.0f, 1.0f)));

  /* Calculate that Y axis using cross(Z, X). */
  EXPECT_EQ(kYAxis4f, Vector4f(0.0f, 0.0f, 1.0f, 0.0f).cross(
                      Vector4f(1.0f, 0.0f, 0.0f, 0.0f)));

  EXPECT_EQ(kYAxis4f, Vector4f(0.0f, 0.0f, 1.0f, 1.0f).cross(
                      Vector4f(1.0f, 0.0f, 0.0f, 0.0f)));

  EXPECT_EQ(kYAxis4f, Vector4f(0.0f, 0.0f, 1.0f, 0.0f).cross(
                      Vector4f(1.0f, 0.0f, 0.0f, 1.0f)));
}

/***************************************************************************//**
 * Test that the dot product is calculated correctly. Diffirent combinations of
 * W component is used to ensure it does not affect the calculation.
 ******************************************************************************/
TEST(Vector4f, DotProduct)
{
  EXPECT_EQ(32, Vector4f(1.0f, 2.0f, 3.0f, 0.0f).dot(
                Vector4f(4.0f, 5.0f, 6.0f, 0.0f)));

  EXPECT_EQ(32, Vector4f(1.0f, 2.0f, 3.0f, 1.0f).dot(
                Vector4f(4.0f, 5.0f, 6.0f, 0.0f)));

  EXPECT_EQ(32, Vector4f(1.0f, 2.0f, 3.0f, 0.0f).dot(
                Vector4f(4.0f, 5.0f, 6.0f, 1.0f)));
}

#endif /* ANUBIS_UNIT_TESTS_VECTOR4F_TESTS_HPP */
