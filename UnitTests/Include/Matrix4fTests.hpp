#ifndef ANUBIS_UNIT_TESTS_MATRIX4F_TESTS_HPP
#define ANUBIS_UNIT_TESTS_MATRIX4F_TESTS_HPP

#include <gtest/gtest.h>
#include "../../Include/Anubis/Math/Matrix4f.hpp"

using namespace Anubis::Math;

/*##############################################################################
 * MATRIX4F TESTS
 * --------------
 *
 *############################################################################*/
TEST(Matrix4f, MultiplyMatrix4fWithMatrix4f)
{
  /* The first matrix. */
  Matrix4f mat1;
  for(int i = 0; i < 16; i++)
  {
    mat1.memory()[i] = i;
  }

  /* The second matrix. */
  Matrix4f mat2;
  for(int i = 16; i < 32; i++)
  {
    mat2.memory()[i-16] = i;
  }

  /* The expected result. */
  Matrix4f result;
  float * mem = result.memory();
  mem[0] = 440; mem[4] = 536; mem[8]  = 632; mem[12] = 728;
  mem[1] = 510; mem[5] = 622; mem[9]  = 734; mem[13] = 846;
  mem[2] = 580; mem[6] = 708; mem[10] = 836; mem[14] = 964;
  mem[3] = 650; mem[7] = 794; mem[11] = 938; mem[15] = 1082;

  /* Run the test. */
  EXPECT_EQ(result, mat1 * mat2);
}

#endif /* ANUBIS_UNIT_TESTS_MATRIX4F_TESTS_HPP */
