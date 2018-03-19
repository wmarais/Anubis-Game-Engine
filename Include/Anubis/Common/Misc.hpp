#ifndef ANUBIS_COMMON_MISC_HPP
#define ANUBIS_COMMON_MISC_HPP

#include "Config.hpp"

#include <cstdint>
#include <cassert>
#include <ostream>
#include <iomanip>
#include <memory>
#include <cmath>
#include <iostream>
#include <cfloat>
#include <map>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <ios>
#include <mutex>
#include <atomic>
#include <thread>
#include <exception>

//#define ANUBIS_HAS_SSE
#define ANUBIS_HOST_IS_LITTLE_ENDIAN

#ifdef ANUBIS_OS_WINDOWS
  #define ANUBIS_DIR_SEPERATOR  '\\'
#else /* ! ANUBIS_OS_WINDOWS */
  #define ANUBIS_DIR_SEPERATOR  '/'
#endif /* ANUBIS_OS_WINDOWS */


namespace Anubis
{
  namespace Common
  {
    class Timer
    {

    public:

      /*********************************************************************//**
       * Return the number of nano seconds since the Epoch.
       *
       * @return The number of nano seconds sicne the epoch.
       ************************************************************************/
      static uint64_t nsSinceEpoch()
      {
        /* Get the time since the epoch and convert it into nano seconds. */
        return std::chrono::duration_cast<std::chrono::nanoseconds>
                 (std::chrono::system_clock::now().time_since_epoch()).count();
      }


    };
  }

  namespace Math
  {
    class Constants
    {
    public:
      template<typename T> static T pi()
      {
        return T(3.14159265358979323846);
      }

      template<typename T> static T twoPi()
      {
        return T(2.0 * 3.14159265358979323846);
      }

    };
  }
}

/*******************************************************************************
 * If SSE is enabled, include the SSE intrinsics header and define the types
 * and function that is needed by Luna.
 ******************************************************************************/
#ifdef ANUBIS_HAS_SSE

  /* Include the required headers. */
  #include <xmmintrin.h>
  #include <smmintrin.h>

  /** Define the simd128_t to __m128 if SSE is available. */
  typedef __m128 simd128_t;

  /** Indicate that SIMD is available if SSE is available. */
  #define ANUBIS_HAS_SIMD

#endif /* ANUBIS_HAS_SSE */

/*******************************************************************************
 * If NEON is enabled, include the NEON intrinsics header and define the types
 * and functions that is needed by Luna.
 ******************************************************************************/
#ifdef ANUBIS_HAS_NEON

  /* Include the noeon headers. */
  #include <arm_neon.h>

  /* Define the simd128_t to float32x4_t if neon is available. */
  typedef float32x4_t simd128_t;

  /* Indicate that SIMD is available. */
  #define ANUBIS_HAS_SIMD

#endif /* ANUBIS_HAS_NEON */

/***************************************************************************//**
 * Specify the required memory alignment (in bytes) for SIMD memory. For optimal
 * performance, 16byte / 128bit alignment must be used. (I seem to recall that
 * some calls will also fail if the memory is not properly aligned.)
 ******************************************************************************/
#define ANUBIS_SIMD_MEM_ALIGNMENT   16

/***************************************************************************//**
 * Force the the function to always be inlined by the compiler.
 ******************************************************************************/
#define ANUBIS_FORCE_INLINE __attribute__((always_inline)) inline

#define ANUBIS_INLINE inline

//#define ANUBIS_FORCE_INLINE __forceinline

#define ANUBIS_THROW_RUNTIME_EXCEPTION(msg) std::cerr << msg << std::endl; exit(EXIT_FAILURE)
//#define ANUBIS_LOG_WARN(msg)

#endif // ANUBIS_COMMON_MISC_HPP
