#ifndef ANUBIS_MATH_QUATERNION_HPP
#define ANUBIS_MATH_QUATERNION_HPP

#include "Vector4f.hpp"
#include "Matrix4f.hpp"

namespace Anubis
{
  namespace Math
  {
    class Quaternion
    {
      static constexpr const float kTolerance = 0.00001f;

      union
      {
        struct
        {
          float fX, fY, fZ, fW;
        };
#ifdef ANUBIS_HAS_SSE
        __m128 fFields;
#endif // ANUBIS_HAS_SSE
      };
    public:

      /*********************************************************************//**
       * Create a quaternion with the specific components.
       *
       * @param x The x component of the quaternion.
       * @param y The y component of the quaternion.
       * @param z The z component of the quaternion.
       * @param w The w component of the quaternion.
       ************************************************************************/
      Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f,
                 float w = 0.0f);

      /*********************************************************************//**
       * Create a quaternion with a rotation about the specific axis.
       *
       * @param vec   The axis to rotate about.
       * @param angle The angle to rotate by.
       ************************************************************************/
      static Quaternion fromAxisAndAngle(const Vector4f & vec, float angle);

      /*********************************************************************//**
       * Create a quarternion with the specific rotatnion angles. All angles are
       * in radians.
       *
       * @param xRot  The rotation about the x axis (pitch).
       * @param yRot  The rotation about the y axis (yaw).
       * @param zRot  The rotation about the z axis (roll).
       ************************************************************************/
      static Quaternion fromEuler(float xRot, float yRot, float zRot);

      /*********************************************************************//**
       * Calculate and return the conjugate of the quaternion without modifying
       * the original quaternion.
       *
       * @return The conjugate.
       ************************************************************************/
      Quaternion conjugate() const;

      /*********************************************************************//**
       * Normalise the quarternion and return a reference to it.
       *
       * @return The reference to this quaternion.
       ************************************************************************/
      Quaternion & normalise();

      /*********************************************************************//**
       * Convert the quaternion into an rotation matrix.
       *
       * @return  A rotation matrix
       ************************************************************************/
      Matrix4f toMatrix();

      /*********************************************************************//**
       * Multiply two quaternions together without modifying either the lhs or
       * rhs quaternions.
       *
       * @param rhs The quaternion of the rhs of the multiplication.
       * @return    The quaternion resulkting from the multiplication.
       ************************************************************************/
      Quaternion operator * (const Quaternion & rhs) const;

      /*********************************************************************//**
       * Apply the rotation of the quaternion to a vector.
       *
       * @param rhs The vector to rotate.
       * @return    The rotated vector.
       ************************************************************************/
      Vector4f operator * (const Vector4f & rhs) const;
    };
  }
}

#endif /* ANUBIS_MATH_QUATERNION_HPP */
