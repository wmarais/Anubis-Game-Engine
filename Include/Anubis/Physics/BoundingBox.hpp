#ifndef ANUBIS_PHYSICS_BOUNDING_BOX_HPP
#define ANUBIS_PHYSICS_BOUNDING_BOX_HPP

#include "../Math/Vector4f.hpp"
#include "BoundingVolume.hpp"

namespace Anubis
{
  namespace Physics
  {
    class BoundingBox final : public BoundingVolume
    {
      /** The points of the bounding mesh. */
      float fX1, fX2, fY1, fY2, fZ1, fZ2;

    public:

      BoundingBox(float x1, float x2, float y1, float y2, float z1, float z2);

      void intersect(const Math::Ray & ray, std::vector<float> & distances);

    };
  }
}

/*Box - Box
Box - Sphere
Box - Mesh
Sphere - Sphere
Sphere - Mesh
Mesh - Mesh*/

#endif /* ANUBIS_PHYSICS_BOUNDING_BOX_HPP */
