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

      /*********************************************************************//**
       * Return a reference to the x coordinate that allow the x coordinate to
       * be both read and written.
       *
       * @return  The read / write reference to the x coordinate.
       ************************************************************************/
      ANUBIS_FORCE_INLINE float & x() noexcept
      {
        return fMem[kX];
      }

      /*********************************************************************//**
       * Return a const reference to the x coordinate that allow the x
       * coordinate to be read only.
       *
       * @return  The read only reference to the x coordinate.
       ************************************************************************/
      ANUBIS_FORCE_INLINE const float & x() const noexcept
      {
        return fMem[kX];
      }

      /*********************************************************************//**
       * Return a reference to the y coordinate that allow the y coordinate to
       * be both read and written.
       *
       * @return  The read / write reference to the y coordinate.
       ************************************************************************/
      ANUBIS_FORCE_INLINE float & y() noexcept
      {
        return fMem[kY];
      }

      /*********************************************************************//**
       * Return a const reference to the y coordinate that allow the y
       * coordinate to be read only.
       *
       * @return  The read only reference to the y coordinate.
       ************************************************************************/
      ANUBIS_FORCE_INLINE const float & y() const noexcept
      {
        return fMem[kY];
      }

    };
  }
}

#endif /* ANUBIS_MATH_VECTOR2F_HPP */
