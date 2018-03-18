#include "../../../Include/Anubis/Physics/BoundingBox.hpp"

using namespace Anubis::Physics;

/******************************************************************************/
BoundingBox::BoundingBox(float x1, float x2, float y1, float y2, float z1,
  float z2) : BoundingVolume(Types::Box), fX1(x1), fX2(x2), fY1(y1), fY2(y2),
  fZ1(z1), fZ2(z2) {}

/******************************************************************************/
void BoundingBox::intersect(const Math::Ray & ray,
                            std::vector<float> & distances)
{

}
