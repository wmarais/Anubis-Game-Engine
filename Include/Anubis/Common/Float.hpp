#ifndef ANUBIS_COMMON_FLOAT_HPP
#define ANUBIS_COMMON_FLOAT_HPP

#include "Misc.hpp"

namespace Anubis
{
  class Float
  {
  public:
    static constexpr const float kPi = 3.1415927f;
    static constexpr const float kTwoPi = 6.2831854f;

    /***********************************************************************//**
     * Compare two floating point numbers to check if they are equal or very
     * close to each other in magnitude.
     *
     *      http://floating-point-gui.de/errors/comparison/
     *
     **************************************************************************/
    static bool compare(float a, float b, float epsilon = 0.00001f);

    /*********************************************************************//**
     * Add the step to the angle and make sure it always in the range of
     * +/- 2*Pi. Since all angles obey the rule Angle +/- 2*Pi = Angle, this
     * is simply a method of ensureing the floating point values does not
     * become excessively large or small leading to problematic floating point
     * rounding errors.
     *
     * @param angle The angle that the step will be added too.
     * @param step  The step to be added.
     * @return      The updated angle range
     **************************************************************************/
    static float sumAngles(float angle1, float angle2);


  };
}

#endif // ANUBIS_COMMON_FLOAT_HPP
