#ifndef ANUBIS_MATH_MATRIX4F_HPP
#define ANUBIS_MATH_MATRIX4F_HPP

#include "Vector4f.hpp"

#include <cstdint>

namespace Anubis
{
  namespace Math
  {
    /***********************************************************************//**
     * Matrix class used for manipulating 3D geomtry and vectors.
     **************************************************************************/
    class Matrix4f final
    {
    public:
      static const size_t kComponentCount = 16;

    private:

      /*********************************************************************//**
       * The index of the first component in the translation column.
       ************************************************************************/
      static const size_t kTranslateColStartIndex = 12;


      /*********************************************************************//**
       * Anonymous union to overlap the SIMD register memory and the packed
       * floating point memory for the matrix components, aligned to 128bit
       * boundary required for SIMD.
       ************************************************************************/
      union
      {
        /** The components of the matrix. */
        float fMem[kComponentCount];

        /*******************************************************************//**
         * Anonymous struct to tightly pack the matrix components into a 128bit
         * aligned block of memory.
         **********************************************************************/
        struct
        {
          Vector4f fRight;
          Vector4f fUp;
          Vector4f fHeading;
          Vector4f fPosition;
        }fVectors;

#ifdef ANUBIS_HAS_SIMD
        /** The SIMD register value. */
        simd128_t fCol0;
        simd128_t fCol1;
        simd128_t fCol2;
        simd128_t fCol3;
#endif /* ANUBIS_HAS_SIMD */
      } __attribute__ ((aligned (ANUBIS_SIMD_MEM_ALIGNMENT)));
    public:

      /*********************************************************************//**
       * Default consntructor configuring the matrix as an Identity Matrix.
       ************************************************************************/
      Matrix4f()
      {
        /* Set the default matrix layout as an Identity Matrix. */
        fMem[0] = 1.0f; fMem[4] = 0.0f; fMem[8]  = 0.0f; fMem[12] = 0.0f;
        fMem[1] = 0.0f; fMem[5] = 1.0f; fMem[9]  = 0.0f; fMem[13] = 0.0f;
        fMem[2] = 0.0f; fMem[6] = 0.0f; fMem[10] = 1.0f; fMem[14] = 0.0f;
        fMem[3] = 0.0f; fMem[7] = 0.0f; fMem[11] = 0.0f; fMem[15] = 1.0f;
      }

      /*********************************************************************//**
       * Copy constructor to perform a deep copy of the matrix memory.
       ************************************************************************/
      Matrix4f(const Matrix4f & cp)
      {
        /* Copy the matrix memory. */
        memcpy(fMem, cp.fMem, sizeof(float) * kComponentCount);
      }

      ANUBIS_FORCE_INLINE void set(size_t row, size_t col, float value)
      {
        fMem[col * 4 + row] = value;
      }


      /*********************************************************************//**
       * Return a constant pointer to the matrix memory.
       ************************************************************************/
      ANUBIS_FORCE_INLINE const float * memory() const
      {
        return fMem;
      }

      /*********************************************************************//**
       * Return a non constant / mutable pointer to the matrix memory. This
       * function is primarily used for testing and should not be used by
       * developers unless they really know what they are doing.
       ************************************************************************/
      ANUBIS_FORCE_INLINE float * memory()
      {
        return fMem;
      }

      /*********************************************************************//**
       * Copy constructor to perform a deep copy of the matrix memory.
       ************************************************************************/
      ANUBIS_FORCE_INLINE void operator = (const Matrix4f & cp)
      {
        /* Copy the matrix memory. */
        memcpy(fMem, cp.fMem, sizeof(float) * kComponentCount);
      }

      /*********************************************************************//**
       * Compare two matrices for equality. Note that this is not the best way
       * to test and this method is only provided for com
       ************************************************************************/
      bool operator == (const Matrix4f & rhs) const
      {
        /* The result of the comparison. */
        bool result = true;

        /* Iterate through all the components and test them. */
        for(size_t i = 0; i < kComponentCount && result == true; i++)
        {
          /* Compare and latch the result. */
          result &= Float::compare(fMem[i], rhs.fMem[i]);
        }

        /* Return the result of the comparison. */
        return result;
      }


      Matrix4f operator * (const Matrix4f & rhs)
      {
        /* The resulting matrix. */
        Matrix4f result;

        /* Calculate the first row. */
        result.fMem[0] =
            fMem[0] * rhs.fMem[0] + fMem[4]  * rhs.fMem[1] +
            fMem[8] * rhs.fMem[2] + fMem[12] * rhs.fMem[3];

        result.fMem[4] =
            fMem[0] * rhs.fMem[4] + fMem[4] *  rhs.fMem[5] +
            fMem[8] * rhs.fMem[6] + fMem[12] * rhs.fMem[7];

        result.fMem[8] =
            fMem[0] * rhs.fMem[8] +  fMem[4] *  rhs.fMem[9] +
            fMem[8] * rhs.fMem[10] + fMem[12] * rhs.fMem[11];

        result.fMem[12] =
            fMem[0] * rhs.fMem[12] + fMem[4] *  rhs.fMem[13] +
            fMem[8] * rhs.fMem[14] + fMem[12] * rhs.fMem[15];

        /* Calculate the second row. */
        result.fMem[1] =
            fMem[1] * rhs.fMem[0] + fMem[5] *  rhs.fMem[1] +
            fMem[9] * rhs.fMem[2] + fMem[13] * rhs.fMem[3];

        result.fMem[5] =
            fMem[1] * rhs.fMem[4] + fMem[5] *  rhs.fMem[5] +
            fMem[9] * rhs.fMem[6] + fMem[13] * rhs.fMem[7];

        result.fMem[9] =
            fMem[1] * rhs.fMem[8] +  fMem[5] *  rhs.fMem[9] +
            fMem[9] * rhs.fMem[10] + fMem[13] * rhs.fMem[11];

        result.fMem[13] =
            fMem[1] * rhs.fMem[12] + fMem[5] *  rhs.fMem[13] +
            fMem[9] * rhs.fMem[14] + fMem[13] * rhs.fMem[15];

        /* Calculate the third row. */
        result.fMem[2] =
            fMem[2] * rhs.fMem[0] + fMem[6] *  rhs.fMem[1] +
            fMem[10] * rhs.fMem[2] + fMem[14] * rhs.fMem[3];

        result.fMem[6] =
            fMem[2] * rhs.fMem[4] + fMem[6] *  rhs.fMem[5] +
            fMem[10] * rhs.fMem[6] + fMem[14] * rhs.fMem[7];

        result.fMem[10] =
            fMem[2] * rhs.fMem[8] +  fMem[6] *  rhs.fMem[9] +
            fMem[10] * rhs.fMem[10] + fMem[14] * rhs.fMem[11];

        result.fMem[14] =
            fMem[2] * rhs.fMem[12] + fMem[6] *  rhs.fMem[13] +
            fMem[10] * rhs.fMem[14] + fMem[14] * rhs.fMem[15];

        /* Calculate the fourth row. */
        result.fMem[3] =
            fMem[3] * rhs.fMem[0] + fMem[7] *  rhs.fMem[1] +
            fMem[11] * rhs.fMem[2] + fMem[15] * rhs.fMem[3];

        result.fMem[7] =
            fMem[3] * rhs.fMem[4] + fMem[7] *  rhs.fMem[5] +
            fMem[11] * rhs.fMem[6] + fMem[15] * rhs.fMem[7];

        result.fMem[11] =
            fMem[3] * rhs.fMem[8] +  fMem[7] *  rhs.fMem[9] +
            fMem[11] * rhs.fMem[10] + fMem[15] * rhs.fMem[11];

        result.fMem[15] =
            fMem[3] * rhs.fMem[12] + fMem[7] *  rhs.fMem[13] +
            fMem[11] * rhs.fMem[14] + fMem[15] * rhs.fMem[15];

        /* Return the result of the multiplication. */
        return result;
      }

      /*********************************************************************//**
       * Create a translation matrix.
       ************************************************************************/
      ANUBIS_FORCE_INLINE static Matrix4f translate(const Vector4f & offset)
      {
        /* Create a default Identity Matrix. */
        Matrix4f result;

        /* Set the translation column. */
        result.setTranslation(offset);

        /* Return the result of the function. */
        return result;
      }

      /*********************************************************************//**
       * Create a scaling matrix.
       ************************************************************************/
      ANUBIS_FORCE_INLINE static Matrix4f scale(const Vector4f & scale)
      {
        /* Create a default Identity Matrix. */
        Matrix4f result;

        /* Set the scaling components. */
        result.fMem[0] = scale.x();
        result.fMem[5] = scale.y();
        result.fMem[10] = scale.z();

        /* Return the result of the function. */
        return result;
      }

      /*********************************************************************//**
       * Create a perspective matrix. This is using the same projection matrix
       * structure as defined in OpenGL.
       ************************************************************************/
      ANUBIS_FORCE_INLINE static Matrix4f perspective(float fovy, float aspect,
                                                      float zNear, float zFar)
      {
        /* The computer perspective matrix. */
        Matrix4f result;

        /* Precalculate the tan to simplify the equation. */
        float tanHalfFov = std::tan(fovy / 2.0f);

        /* Calculate all the matrix components. */
        result.fMem[0] = 1.0f / (aspect * tanHalfFov);
        result.fMem[5] = 1.0f / tanHalfFov;
        result.fMem[14] = -1.0f;
        result.fMem[10] = -(zFar + zNear) / (zFar - zNear);
        result.fMem[11] = -(2.0f * zFar * zNear) / (zFar - zNear);

        /* Return the calculated matrix. */
        return result;
      }

      /*********************************************************************//**
       * Set the translation column in the matrix.
       ************************************************************************/
      ANUBIS_FORCE_INLINE void setTranslation(const Vector4f & offset)
      {
        /* Copy the translation vector's components into the correct column. */
        memcpy(fMem + kTranslateColStartIndex, offset.memory(),
               sizeof(float) * Vector4f::kComponentCount);
      }

      ANUBIS_FORCE_INLINE const Vector4f & upVector()
      {
        return fVectors.fUp;
      }

      ANUBIS_FORCE_INLINE const Vector4f & rightVector()
      {
        return fVectors.fRight;
      }

      ANUBIS_FORCE_INLINE const Vector4f & headingVector()
      {
        return fVectors.fHeading;
      }

      /*********************************************************************//**
       * Implements the matrix and vector multiplication operator.
       ************************************************************************/
      ANUBIS_FORCE_INLINE Vector4f operator * (const Vector4f & rhs)
      {
        /* Return the calculated vector.*/
        return Vector4f
        (
          /* Calculate the X component. */
          fMem[0] * rhs.x() + fMem[4]  * rhs.y() +
          fMem[8] * rhs.z() + fMem[12] * rhs.w(),

          /* Calculate the Y Component. */
          fMem[1] * rhs.x() + fMem[5] * rhs.y() +
          fMem[9] * rhs.z() + fMem[13] * rhs.w(),

          /* Calculate the Z component. */
          fMem[2] * rhs.x() + fMem[6] * rhs.y() +
          fMem[10] * rhs.z() + fMem[14] * rhs.w(),

          /* Calculate the W component. */
          fMem[3] * rhs.x() + fMem[7]  * rhs.y() +
          fMem[11] * rhs.z() + fMem[15] * rhs.w()
        );
      }
    };
  }
}

/***************************************************************************//**
 * Print the matrix to an output stream.
 *
 * @param   os  The output stream to print the vector to.
 * @param   mat The matrix to print.
 * @return      The output stream that was passed in (os).
 ******************************************************************************/
ANUBIS_FORCE_INLINE std::ostream & operator << (std::ostream & os,
                            const Anubis::Math::Matrix4f & mat)
{
  /* Get a handle to the matrix's memory. */
  const float * m = mat.memory();

  os << std::scientific << std::setw( 11 ) << std::setprecision( 3 );
  os << "+-                                           -+\n";
  os << "| " << m[0] << " " << m[4] << " " << m[8]  << " " << m[12] << " |\n"
     << "| " << m[1] << " " << m[5] << " " << m[9]  << " " << m[13] << " |\n"
     << "| " << m[2] << " " << m[6] << " " << m[10] << " " << m[14] << " |\n"
     << "| " << m[3] << " " << m[7] << " " << m[11] << " " << m[15] << " |\n";
  os << "+-                                           -+\n";

  /* Flush the output. */
  os << std::endl;

  /* Return the reference to the ostream. */
  return os;
}

#endif /* ANUBIS_MATH_MATRIX4F_HPP */
