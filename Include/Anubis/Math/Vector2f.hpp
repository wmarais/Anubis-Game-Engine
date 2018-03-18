#ifndef ANUBIS_MATH_VECTOR2F_HPP
#define ANUBIS_MATH_VECTOR2F_HPP

#include "../Common/Misc.hpp"

namespace Anubis
{
  namespace Math
  {
    class Vector2f
    {
      float fMem[2];

      static constexpr const size_t kX = 0;
      static constexpr const size_t kY = 0;

    public:
      ANUBIS_FORCE_INLINE Vector2f(float x, float y)
      {
        fMem[kX] = x;
        fMem[kY] = y;
      }
    };
  }
}

#endif /* ANUBIS_MATH_VECTOR2F_HPP */
