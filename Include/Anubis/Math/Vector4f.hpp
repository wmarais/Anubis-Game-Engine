/***************************************************************************//**
 * @brief     Homogeneous 3D vector class.
 * @details   Implements all common vector operations required for 3D geometry
 *            calculations and manipulations. Homogeneous coordinates are used
 *            and if enabled, SIMD optimisations are used.
 * @file      Vector4f.hpp
 * @author    Wynand Marais
 * @version   1.0.0
 * @copyright WM Software Product License
 ******************************************************************************/
#ifndef ANUBIS_MATH_VECTOR4_HPP
#define ANUBIS_MATH_VECTOR4_HPP

#include "../Common/Misc.hpp"
#include "../Common/Float.hpp"

/***************************************************************************//**
 * Homogeneous vector class used for 3D manipulations in a projective
 * geometry space.
 ******************************************************************************/
#define ANUBIS_VEC4F_MAX_ULP    ((int32_t)1)

namespace Anubis
{
  namespace Math
  {
    /***********************************************************************//**
     * Homogeneous vector class used for 3D manipulations in a projective
     * geometry space.
     **************************************************************************/
    class Vector4f final
    {
    public:
      /** The number of vector components. */
      static const size_t kComponentCount = 4;

      /** The index of the X component in the fMem array. */
      static const size_t kX = 0;

      /** The index of the Y component in the fMem array. */
      static const size_t kY = 1;

      /** The index of the Z component in the fMem array. */
      static const size_t kZ = 2;

      /** The index of the W component in the fMem array. */
      static const size_t kW = 3;

    private:


      /*************************************************************************
       * Anonymous union to overlap the SIMD register memory and the packed
       * floating point memory for the vector coordinates, aligned to 128bit
       * boundary required for SIMD.
       ************************************************************************/
      union
      {
        /** The floating point memory required to store all the vector
         * components */
        float fMem[kComponentCount];
#ifdef ANUBIS_HAS_SIMD
        /** The SIMD register value. */
        simd128_t fSIMD;
#endif /* ANUBIS_HAS_SIMD */
      } __attribute__ ((aligned (ANUBIS_SIMD_MEM_ALIGNMENT)));
    public:

      /*********************************************************************//**
       * Initialise the components of the vector. The general rule is that for
       * positions / vertexes, set w to 1.0f, for directions / vectors, set
       * w to 0.0f.
       *
       * @param[in]   x The value that the vector's X coordinate will be
       *                initialised too. The default is 0.0f.
       * @param[in]   y The value that the vector's Y coordinate will be
       *                initialised too. The default is 0.0f.
       * @param[in]   z The value that the vector's Z coordinate will be
       *                initialised too. The default is 0.0f.
       * @param[in]   w The value that the vector's W coordinate will be
       *                initialised too. The default is 0.0f.
       ************************************************************************/
      ANUBIS_FORCE_INLINE Vector4f(float x = 0.0f, float y = 0.0f,
                                   float z = 0.0f, float w = 0.0f) noexcept
      {
        /* The component values. */
        fMem[kX] = x;
        fMem[kY] = y;
        fMem[kZ] = z;
        fMem[kW] = w;

        #ifdef ANUBIS_HAS_SIMD
          assert(((uint64_t)&fSIMD) % ANUBIS_SIMD_MEM_ALIGNMENT == 0 &&
                 "Vector4f memory not aligned to 128 bit boundary.");
        #endif /* ANUBIS_HAS_SIMD */

      }

      ANUBIS_FORCE_INLINE static Vector4f makePosition(
          float x, float y, float z) noexcept
      {
        return Vector4f(x, y, z, 1.0f);
      }

      ANUBIS_FORCE_INLINE static Vector4f makeDirection(
          float x, float y, float z) noexcept
      {
        return Vector4f(x, y, z, 0.0f);
      }

      ANUBIS_FORCE_INLINE const float * memory() const
      {
        return fMem;
      }

//      /*********************************************************************//**
//       * Set the w coordinate to 1.0f indicating a Position / Vertex.
//       ************************************************************************/
//      ANUBIS_FORCE_INLINE void makePosition() noexcept
//      {
//        fMem[kW] = 1.0f;
//      }

//      /*********************************************************************//**
//       * Set the w coordinate to 0.0f indicating a Direction / Vector.
//       ************************************************************************/
//      ANUBIS_FORCE_INLINE void makeDirection() noexcept
//      {
//        fMem[kW] = 0.0f;
//      }

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

      /*********************************************************************//**
       * Return a reference to the z coordinate that allow the z coordinate to
       * be both read and written.
       *
       * @return  The read / write reference to the z coordinate.
       ************************************************************************/
      ANUBIS_FORCE_INLINE float & z() noexcept
      {
        return fMem[kZ];
      }

      /*********************************************************************//**
       * Return a const reference to the z coordinate that allow the z
       * coordinate to be read only.
       *
       * @return  The read only reference to the z coordinate.
       ************************************************************************/
      ANUBIS_FORCE_INLINE const float & z() const noexcept
      {
        return fMem[kZ];
      }

      /*********************************************************************//**
       * Return a reference to the w coordinate that allow the w coordinate to
       * be both read and written.
       *
       * @return  The read / write reference to the w coordinate.
       ************************************************************************/
      ANUBIS_FORCE_INLINE float & w() noexcept
      {
        return fMem[kW];
      }

      /*********************************************************************//**
       * Return a const reference to the w coordinate that allow the w
       * coordinate to be read only.
       *
       * @return  The read only reference to the w coordinate.
       ************************************************************************/
      ANUBIS_FORCE_INLINE const float & w() const noexcept
      {
        return fMem[kW];
      }

      /*********************************************************************//**
       * Calculate the length of the vector using the x, y and z components and
       * discarding the w component.
       *
       * <B>SSE Version Requirements:</B>
       *
       *  Function     | SSE Version |
       *  :------------|:-----------:|
       *  _mm_dp_ps    |     4.1     |
       *  _mm_sqrt_ss  |     1.0     |
       *  _mm_mul_ps   |     1.0     |
       *
       * @return The length of the 3D vector.
       ************************************************************************/
      ANUBIS_FORCE_INLINE float length() const noexcept
      {
        #ifdef ANUBIS_HAS_SSE
          return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(fSIMD, fSIMD, 0x71)));
        #endif /* ANUBIS_HAS_SSE4 */

        #ifndef ANUBIS_HAS_SIMD
          return sqrt(fMem[kX] * fMem[kX] +
                      fMem[kY] * fMem[kY] +
                      fMem[kZ] * fMem[kZ]);
        #endif /* ANUBIS_HAS_SIMD */
      }

      /*********************************************************************//**
       * Normalise the vector as fast as possible with lower accuracy. This
       * function is only faster when SIMD is used, otherwise it has the same
       * performance as normalise(). The function uses reciprocal square root
       * which allows for the user multiplication instead of a slower division,
       * however the reciprocal is only an approximation and if accuracy is
       * required, use normalise() instead.
       *
       * <B>SSE Version Requirements:</B>
       *
       * Function     | SSE Version |
       * :------------|:-----------:|
       * _mm_dp_ps    |      4.1    |
       * _mm_rsqrt_ps |      1.0    |
       * _mm_mul_ps   |      1.0    |
       * _mm_blend_ps |      4.1    |
       ************************************************************************/
      ANUBIS_FORCE_INLINE void normaliseFast() noexcept
      {
        #ifdef ANUBIS_HAS_SSE
          simd128_t reciprocal = _mm_rsqrt_ps(_mm_dp_ps(fSIMD, fSIMD, 0x77));
          fSIMD = _mm_blend_ps(_mm_mul_ps(fSIMD, reciprocal), fSIMD, 0x08);
        #endif /* ANUBIS_HAS_SSE */

        #ifndef ANUBIS_HAS_SIMD
          float len = length();
          fMem[kX] /= len;
          fMem[kY] /= len;
          fMem[kZ] /= len;
        #endif /* ANUBIS_HAS_SIMD */
      }

      /*********************************************************************//**
       * Accurately normalise the vector. This is slower than normaliseFast()
       * but is more accurate.
       *
       * <B>SSE Version Requirements:</B>
       *
       * Function    | SSE Version |
       * :-----------|:-----------:|
       * _mm_dp_ps   |     4.1     |
       * _mm_sqrt_ps |     1.0     |
       * _mm_div_ps  |     1.0     |
       ************************************************************************/
      ANUBIS_FORCE_INLINE void normalise() noexcept
      {
        #ifdef ANUBIS_HAS_SSE
          simd128_t normal = _mm_sqrt_ps(_mm_dp_ps(fSIMD, fSIMD, 0x7F));
          fSIMD = _mm_blend_ps(_mm_div_ps(fSIMD, normal), fSIMD, 0x08);
        #endif /* ANUBIS_HAS_SSE */

        #ifndef ANUBIS_HAS_SIMD
          float len = length();
          fMem[kX] /= len;
          fMem[kY] /= len;
          fMem[kZ] /= len;
        #endif /* ANUBIS_HAS_SIMD */
      }

      /*********************************************************************//**
       * Calculate the cross product of the vector. The w component are not used
       * for the calculation and is set to 0.0f for the returned vector.
       *
       * <B>Example (Left Handed Coordinate System)</B>
       *
       * For a Left Handed coordinate system, the various axis / direction
       * vectors can be calculate as:
       *
       * @code
       *  // Generally the right vector.
       *  Vector4f xAxis = yAxis.cross(zAxis);
       *
       *  // Generally the up vector.
       *  Vector4f yAxis = zAxis.cross(xAxis);
       *
       *  // Generally the heading vector.
       *  Vector4f zAxis = xAxis.cross(yAxis);
       * @endcode
       *
       * <B>Example (Right Handed Coordinate System)</B>
       *
       * For a Right Handed coordinate system, the various axis / direction
       * vectors can be calculate as:
       *
       * @code
       *  // Generally the right vector.
       *  Vector4f xAxis = zAxis.cross(zAxis);
       *
       *  // Generally the up vector.
       *  Vector4f yAxis = zAxis.cross(xAxis);
       *
       *  // Generally the heading vector.
       *  Vector4f zAxis = xAxis.cross(yAxis);
       * @endcode
       *
       * <B>SSE Version Requirements:</B>
       *
       * Function       | SSE Version |
       * :--------------|:-----------:|
       * _mm_shuffle_ps |     1.0     |
       * _mm_mul_ps     |     1.0     |
       *
       * @param   rhs The vector on the right hand side of the cross product.
       * @return
       ************************************************************************/
      ANUBIS_FORCE_INLINE Vector4f cross(const Vector4f & rhs) const noexcept
      {
        /* The resulting vector. */
        Vector4f result;

        #ifdef ANUBIS_HAS_SSE
          result.fSIMD =  _mm_sub_ps(
              _mm_mul_ps(_mm_shuffle_ps(fSIMD, fSIMD, _MM_SHUFFLE(3, 0, 2, 1)),
                         _mm_shuffle_ps(rhs.fSIMD, rhs.fSIMD,
                                        _MM_SHUFFLE(3, 1, 0, 2))),
              _mm_mul_ps(_mm_shuffle_ps(fSIMD, fSIMD, _MM_SHUFFLE(3, 1, 0, 2)),
                         _mm_shuffle_ps(rhs.fSIMD, rhs.fSIMD,
                                        _MM_SHUFFLE(3, 0, 2, 1))));
        #endif /* ANUBIS_HAS_SSE4 */

        #ifndef ANUBIS_HAS_SIMD
          result.fMem[kX] = fMem[kY] * rhs.fMem[kZ] - fMem[kZ] * rhs.fMem[kY];
          result.fMem[kY] = fMem[kZ] * rhs.fMem[kX] - fMem[kX] * rhs.fMem[kZ];
          result.fMem[kZ] = fMem[kX] * rhs.fMem[kY] - fMem[kY] * rhs.fMem[kX];
        #endif /* ANUBIS_HAS_SIMD */

        /* Return the result. */
        return result;
      }

      ANUBIS_FORCE_INLINE float dot(const Vector4f & rhs) const noexcept
      {
        return  fMem[kX] * rhs.fMem[kX] +
                fMem[kY] * rhs.fMem[kY] +
                fMem[kZ] * rhs.fMem[kZ];
      }

      /*********************************************************************//**
       * Add this vector to the rhs vector and return the result without
       * modifying either vector. The W component of the returned vector matches
       * the W component of the lhs vector.
       *
       * @param rhs The vector to be added to this vector.
       * @return    The result of the vector addition.
       ************************************************************************/
      ANUBIS_FORCE_INLINE Vector4f operator + (const Vector4f & rhs)
        const noexcept
      {
        /* The result of the function. */
        Vector4f result;

        #ifdef ANUBIS_HAS_SSE
          result.fSIMD = _mm_blend_ps(fSIMD, _mm_add_ps(fSIMD, rhs.fSIMD),
                                      0x07);
        #endif /* ANUBIS_HAS_SSE */

        #ifndef ANUBIS_HAS_SIMD
          result.fMem[kX] = fMem[kX] + rhs.fMem[kX];
          result.fMem[kY] = fMem[kY] + rhs.fMem[kY];
          result.fMem[kZ] = fMem[kZ] + rhs.fMem[kZ];
          result.fMem[kW] = fMem[kW];
        #endif /* ANUBIS_HAS_SIMD */

          /* Return the result of the function. */
          return result;
      }

      /*********************************************************************//**
       * Add two vectors together storing the result in the lhs vector. The W
       * component of the lhs vector remains unaffected by the operation.
       ************************************************************************/
      ANUBIS_FORCE_INLINE Vector4f & operator += (const Vector4f & rhs) noexcept
      {
        #ifdef ANUBIS_HAS_SSE
          fSIMD = _mm_blend_ps(fSIMD, _mm_add_ps(fSIMD, rhs.fSIMD), 0x07);
        #endif /* ANUBIS_HAS_SSE */

        #ifndef ANUBIS_HAS_SIMD
          fMem[kX] += rhs.fMem[kX];
          fMem[kY] += rhs.fMem[kY];
          fMem[kZ] += rhs.fMem[kZ];
        #endif /* ANUBIS_HAS_SIMD */

        /* Return the reference to this object. */
        return *this;
      }

      /*********************************************************************//**
       * Subtract one vector from another. The W component of the result matches
       * the W component of the lhs vector in the calculation. Neither the lhs
       * or rhs vector is modified during the operation.
       ************************************************************************/
      ANUBIS_FORCE_INLINE Vector4f operator - (const Vector4f & rhs)
        const noexcept
      {
        /* The result of the function. */
        Vector4f result;

        #ifdef ANUBIS_HAS_SSE
          result.fSIMD = _mm_blend_ps(fSIMD, _mm_sub_ps(fSIMD, rhs.fSIMD),
                                      0x07);
        #endif /* ANUBIS_HAS_SSE */

        #ifndef ANUBIS_HAS_SIMD
          result.fMem[kX] = fMem[kX] - rhs.fMem[kX];
          result.fMem[kY] = fMem[kY] - rhs.fMem[kY];
          result.fMem[kZ] = fMem[kZ] - rhs.fMem[kZ];
          result.fMem[kW] = fMem[kW];
        #endif /* ANUBIS_HAS_SIMD */

        /* Return the result of the function. */
        return result;
      }


      /*********************************************************************//**
       * Subtract the rhs vector from this vector.
       ************************************************************************/
      ANUBIS_FORCE_INLINE Vector4f & operator -= (const Vector4f & rhs) noexcept
      {
        #ifdef ANUBIS_HAS_SSE
          fSIMD = _mm_blend_ps(fSIMD, _mm_sub_ps(fSIMD, rhs.fSIMD), 0x07);
        #endif /* ANUBIS_HAS_SSE */

        #ifndef ANUBIS_HAS_SIMD
          fMem[kX] -= rhs.fMem[kX];
          fMem[kY] -= rhs.fMem[kY];
          fMem[kZ] -= rhs.fMem[kZ];
        #endif /* ANUBIS_HAS_SIMD */

        /* Return the reference to the object. */
        return *this;
      }

      /*********************************************************************//**
       * Multiply the x, y and z components by the rhs value leaving the w
       * component unaffected. The returned vector has the same same w
       * component value as this vector.
       ************************************************************************/
      ANUBIS_FORCE_INLINE Vector4f operator * (float rhs) const noexcept
      {
        /* The result of the function. */
        Vector4f result;

        #ifdef ANUBIS_HAS_SSE
          result.fSIMD = _mm_blend_ps(fSIMD, _mm_mul_ps(fSIMD,
                            _mm_setr_ps(rhs, rhs, rhs, 1.0f)), 0x07);
        #endif /* ANUBIS_HAS_SSE */

        #ifndef ANUBIS_HAS_SIMD
          result.fMem[kX] = fMem[kX] * rhs;
          result.fMem[kY] = fMem[kY] * rhs;
          result.fMem[kZ] = fMem[kZ] * rhs;
          result.fMem[kW] = fMem[kW];
        #endif /* ANUBIS_HAS_SIMD */

        /* Return the result of the function. */
        return result;
      }

      /*********************************************************************//**
       * Multiply the x, y and z components by the rhs value leaving the w
       * component unaffected. The returned vector has the same same w
       * component value as this vector.
       ************************************************************************/
      ANUBIS_FORCE_INLINE Vector4f & operator *= (float rhs) noexcept
      {
        #ifdef ANUBIS_HAS_SSE
          fSIMD = _mm_blend_ps(fSIMD, _mm_mul_ps(fSIMD,
                            _mm_setr_ps(rhs, rhs, rhs, 1.0f)), 0x07);
        #endif /* ANUBIS_HAS_SSE */

        #ifndef ANUBIS_HAS_SIMD
          fMem[kX] *= rhs;
          fMem[kY] *= rhs;
          fMem[kZ] *= rhs;
        #endif /* ANUBIS_HAS_SIMD */

        /* Return the reference to the object. */
        return *this;
      }

      /*********************************************************************//**
       * Multiply the x, y and z components by the rhs value leaving the w
       * component unaffected. The returned vector has the same same w
       * component value as this vector.
       ************************************************************************/
      ANUBIS_FORCE_INLINE Vector4f operator / (float rhs) const noexcept
      {
        /* The result of the function. */
        Vector4f result;

        #ifdef ANUBIS_HAS_SSE
          result.fSIMD = _mm_blend_ps(fSIMD, _mm_div_ps(fSIMD,
                            _mm_setr_ps(rhs, rhs, rhs, 1.0f)), 0x07);
        #endif /* ANUBIS_HAS_SSE */

        #ifndef ANUBIS_HAS_SIMD
          result.fMem[kX] = fMem[kX] / rhs;
          result.fMem[kY] = fMem[kY] / rhs;
          result.fMem[kZ] = fMem[kZ] / rhs;
          result.fMem[kW] = fMem[kW];
        #endif /* ANUBIS_HAS_SIMD */

          /* Return the result of the function. */
          return result;
      }

      /*********************************************************************//**
       * Divide the x, y and z components by the rhs value leaving the w
       * component unaffected.
       ************************************************************************/
      ANUBIS_FORCE_INLINE Vector4f & operator /= (float rhs) noexcept
      {
        #ifdef ANUBIS_HAS_SSE
          fSIMD = _mm_blend_ps(fSIMD, _mm_div_ps(fSIMD,
                            _mm_setr_ps(rhs, rhs, rhs, 1.0f)), 0x07);
        #endif /* ANUBIS_HAS_SSE */

        #ifndef ANUBIS_HAS_SIMD
          fMem[kX] /= rhs;
          fMem[kY] /= rhs;
          fMem[kZ] /= rhs;
        #endif /* ANUBIS_HAS_SIMD */

        /* Return the reference to the object. */
        return *this;
      }

      /*********************************************************************//**
       *
       ************************************************************************/
      ANUBIS_FORCE_INLINE bool operator == (const Vector4f & rhs) const noexcept
      {
        /* The status of the function. */
        bool status = false;

//#ifdef ANUBIS_HAS_SSE4
//        int result = _mm_movemask_ps(_mm_cmpeq_ps(fSIMD, rhs.fSIMD));

////        std::cout << "Result = " << result << std::endl;

//        /* Directly compare the values and see if they are valid. */
//        if(result == 0xF)
//        {
//          status = true;
//        }
//        else
//        {
//          /*_mm_cmple_epi32(_mm_setr_epi32(ANUBIS_VEC4F_MAX_ULP,
//                                      ANUBIS_VEC4F_MAX_ULP,
//                                      ANUBIS_VEC4F_MAX_ULP,
//                                      ANUBIS_VEC4F_MAX_ULP),

//                _mm_abs_epi32(_mm_sub_epi32(_mm_load_si128((__m128i*)&fSIMD),
//                                  _mm_load_si128((__m128i*)&(rhs.fSIMD)))));

//           _mm_andnot_si128(_mm_cmpgt_epi32(a, b), _mm_set_epi32(~0, ~0, ~0, ~0));*/
//        }


////        if(1)
////        {
////          status = true;
////        }
////        else
////        {
////          /*_mm_abs_epi32(_mm_sub_epi32(_mm_load_si128((int32_t*)&fSIMD),
////                        _mm_load_si128((int32_t*)&(rhs.fSIMD))));*/




////        }


//          /*fSIMD = _mm_blend_ps(fSIMD, _mm_div_ps(fSIMD,
//                            _mm_setr_ps(rhs, rhs, rhs, 1.0f)), 0x07);*/
//        #endif /* ANUBIS_HAS_SSE */


//        #ifndef ANUBIS_HAS_SIMD
          /* Check if the direct comparison pass. */
          if (Float::compare(fMem[kX], rhs.fMem[kX]) &&
              Float::compare(fMem[kY], rhs.fMem[kY]) &&
              Float::compare(fMem[kZ], rhs.fMem[kZ]) &&
              Float::compare(fMem[kW], rhs.fMem[kW]))
          {
            status = true;
          }
//        #endif /* ANUBIS_HAS_SIMD */

        /* Return the status of the function. */
        return status;
      }
    };

    static const Vector4f kXAxis4f(1.0f, 0.0f, 0.0f, 0.0f);
    static const Vector4f kYAxis4f(0.0f, 1.0f, 0.0f, 0.0f);
    static const Vector4f kZAxis4f(0.0f, 0.0f, 1.0f, 0.0f);
  }
}

/***************************************************************************//**
 * Print the vector to an output stream in the form [x, y, z, w].
 *
 * @param   os  The output stream to print the vector to.
 * @param   vec The vector to print.
 * @return      The output stream that was passed in (os).
 ******************************************************************************/
inline std::ostream & operator << (std::ostream & os,
                            const Anubis::Math::Vector4f & vec)
{
  /* Print the vector. */
  os << "[" << vec.x() << ", " << vec.y() << ", " << vec.z() << ", " << vec.w()
     << "]";

  /* Return the reference to the ostream. */
  return os;
}

#endif /* ANUBIS_MATH_VECTOR4_HPP */
