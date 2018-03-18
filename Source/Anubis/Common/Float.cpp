#include "../../../Include/Anubis/Common/Float.hpp"

using namespace Anubis;

/******************************************************************************/
bool Float::compare(float a, float b, float epsilon)
{
  float absA = std::abs(a);
  float absB = std::abs(b);
  float diff = std::abs(a - b);

  /* Shortcut, handles infinities. */
  if (a == b)
  {
    return true;
  }
  else if (a == 0 || b == 0 || diff < FLT_MIN)
  {
    /* a or b is zero or both are extremely close to it relative error is
     * less meaningful here. */
    return diff < (epsilon * FLT_MIN);
  }

  /* Use relative error. */
  return diff / std::min((absA + absB), FLT_MAX) < epsilon;
}

/******************************************************************************/
float Float::sumAngles(float angle1, float angle2)
{
  /* Add the step to the x rotation. */
  angle1 += angle2;

  /* Make sure the angle is less than 2Pi. */
  while(angle1 > kTwoPi)
  {
    angle1 -= kTwoPi;
  }

  /* Check reduce the angle to greater than -2Pi. */
  while(angle1 < -kTwoPi)
  {
    angle1 += kTwoPi;
  }

  /* Return the angle. */
  return angle1;
}
