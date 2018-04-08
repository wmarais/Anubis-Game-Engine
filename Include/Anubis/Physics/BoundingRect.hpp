#ifndef ANUBIS_PHYSICS_BOUNDING_RECT_HPP
#define ANUBIS_PHYSICS_BOUNDING_RECT_HPP

#include "../Common.hpp"

namespace Anubis
{
  namespace Physics
  {
    class BoundingRect
    {
      float fX, fY, fWidth, fHeight;

    public:
      BoundingRect(float x = 0, float y = 0, float width = 0, float height = 0) :
        fX(x), fY(y), fWidth(width), fHeight(height) {}

      ANUBIS_INLINE bool canFit(const BoundingRect & br) const
      {
        return (br.width() <= fWidth) && (br.height() <= fHeight);
      }

      ANUBIS_FORCE_INLINE float x() const
      {
        return fX;
      }

      ANUBIS_FORCE_INLINE float y() const
      {
        return fY;
      }

      ANUBIS_FORCE_INLINE float width() const
      {
        return fWidth;
      }

      ANUBIS_FORCE_INLINE float height() const
      {
        return fHeight;
      }
    };
  }
}

#endif /* ANUBIS_PHYSICS_BOUNDING_RECT_HPP */
