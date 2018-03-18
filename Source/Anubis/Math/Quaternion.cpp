#include "../../../Include/Anubis/Math/Quaternion.hpp"

using namespace Anubis::Math;

/******************************************************************************/
Quaternion::Quaternion(float x, float y, float z, float w) : fX(x), fY(y),
  fZ(z), fW(w) {}

/******************************************************************************/
Quaternion Quaternion::fromEuler(float xRot, float yRot, float zRot)
{
  /* Scale the angles. Use the same notation as the documentation, a= Alpha,
   * b=beta, g=gama. */
  float a = xRot / 2.0f;
  float b = yRot / 2.0f;
  float g = zRot / 2.0f;

  /* Precompute and cache common trig identities to avoid expensive
   * recalculations. */
  float sina = std::sin(a);
  float sinb = std::sin(b);
  float sing = std::sin(g);
  float cosa = std::cos(a);
  float cosb = std::cos(b);
  float cosg = std::cos(g);

  /* Build the quaternion. */
  float w = cosg * cosb * cosa + sing * sinb * sing;
  float x = cosg * cosb * sina - sing * sinb * cosa;
  float y = cosg * sinb * cosa + cosb * sing * sina;
  float z = cosb * sing * cosa - cosg * sinb * sina;

  /* Normalise the quaternion. */
  return Quaternion(x, y, z, w).normalise();
}

/******************************************************************************/
Quaternion Quaternion::fromAxisAndAngle(const Vector4f & vec, float angle)
{
  /* Scale the angle. */
  angle *= 0.5f;

  /* Calculate the sin of the angle. */
  float sa = std::sin(angle);

  /* Build the quaternion. */
  float x = vec.x() * sa;
  float y = vec.y() * sa;
  float z = vec.z() * sa;
  float w = std::cos(angle);

  /* Normalise and return the quaternion. */
  return Quaternion(x, y, z, w).normalise();
}

/******************************************************************************/
Quaternion & Quaternion::normalise()
{
  /* Calculate the sum inside the square-root. */
  float mag = (fX * fX) + (fY * fY) + (fZ * fZ) + (fW * fW);

  /* Check if the vector is either 0, or allready normalised, in which case
   * leave it alone. */
  if(Float::compare(mag, 0.0f) || Float::compare(mag, 1.0f))
  {
    /* Nothing left to do. */
    return *this;
  }

  /* Calculate the square root. */
  mag = std::sqrt(mag);
  fX /= mag;
  fY /= mag;
  fZ /= mag;
  fW /= mag;

  return *this;
}

/******************************************************************************/
Quaternion Quaternion::conjugate() const
{
  return Quaternion(-fX, -fY, -fZ, fW);
}

/******************************************************************************/
Matrix4f Quaternion::toMatrix()
{
  /* The matrix to be returned. */
  Matrix4f out;

  /* Make sure the quaternion is normalised. */
  normalise();

  /* Calculate some common factors. */
  float x2 = fX * fX;
  float y2 = fY * fY;
  float z2 = fZ * fZ;
  float xy = fX * fY;
  float xz = fX * fZ;
  float yz = fY * fZ;
  float wx = fW * fX;
  float wy = fW * fY;
  float wz = fW * fZ;

  /* Build the final column major matrix (direct opengl style). */
  out.set(0, 0, 1.0f - 2.0f * (y2 + z2));
  out.set(1, 0, 2.0f * (xy - wz));
  out.set(2, 0, 2.0f * (xz + wy));
  out.set(3, 0, 0.0f);

  out.set(0, 1, 2.0f * (xy + wz));
  out.set(1, 1, 1.0f - 2.0f * (x2 + z2));
  out.set(2, 1, 2.0f * (yz - wx));
  out.set(3, 1, 0.0f);

  out.set(0, 2, 2.0f * (xz - wy));
  out.set(1, 2, 2.0f * (yz + wx));
  out.set(2, 2, 1.0f - 2.0f * (x2 + y2));
  out.set(3, 2, 0.0f);

  out.set(0, 3, 0.0f);
  out.set(1, 3, 0.0f);
  out.set(2, 3, 0.0f);
  out.set(3, 3, 1.0f);

  /* Return the calculated matrix. */
  return out;
}

/******************************************************************************/
Quaternion Quaternion::operator * (const Quaternion & rhs) const
{
  Quaternion out;

  out.fX = fW * rhs.fX + fX * rhs.fW + fY * rhs.fZ - fZ * rhs.fY;
  out.fY = fW * rhs.fY + fY * rhs.fW + fZ * rhs.fX - fX * rhs.fZ;
  out.fZ = fW * rhs.fZ + fZ * rhs.fW + fX * rhs.fY - fY * rhs.fX;
  out.fW = fW * rhs.fW - fX * rhs.fX - fY * rhs.fY - fZ * rhs.fZ;

  return out;
}

/******************************************************************************/
Vector4f Quaternion::operator * (const Vector4f & rhs) const
{
  /* Build the vector into a quaternion. */
  Quaternion vq(rhs.x(), rhs.y(), rhs.z(), 0.0f);

  /* Calculate the transformation of the vector. */
  Quaternion rq =  vq * conjugate();
  rq = (*this) * rq;

  /* Return the transformed vector. */
  return {rq.fX, rq.fY, rq.fZ, 0.0f};
}
