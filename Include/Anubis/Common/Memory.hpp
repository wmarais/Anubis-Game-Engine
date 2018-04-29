#ifndef ANUBIS_COMMON_MEMORY_HPP
#define ANUBIS_COMMON_MEMORY_HPP

#include "Misc.hpp"

/***************************************************************************//**
 * Global equal operator overload allowing the comarison of shared pointers and
 * raw pointers with the shared_ptr as the lhs value and the raw pointer as the
 * rhs value.
 ******************************************************************************/
template <class T> ANUBIS_FORCE_INLINE
bool operator == (const std::shared_ptr<T>& lhs, const T * const rhs) noexcept
{
    return lhs.get() == rhs;
}

/***************************************************************************//**
 * Global equal operator overload allowing the comarison of shared pointers and
 * raw pointers with the shared_ptr as the rhs value and the raw pointer as the
 * lhs value.
 ******************************************************************************/
template <class T> ANUBIS_FORCE_INLINE
bool operator==(const T * const lhs, const std::shared_ptr<T>& rhs) noexcept
{
    return lhs == rhs.get();
}

namespace Anubis
{
  namespace Common
  {
    class Memory
    {
    public:

      /** The memory alignment to use for SSE SIMD data. */
      static const size_t kSIMDAlignment = 16;

      /** The memory alignment to use for AVX SIMD data. */
      /* static const size_t kSIMDAlignment = 32; */

      /** The memory alignment to use for GPU data. */
      static const size_t kGPUAlignment = 4;

      template <typename T>
      ANUBIS_INLINE T rangeEnd(T start, T end)
      {
        /* The current iterator used for searching. */
        T cur = start;

        /* Search for the end of range. */
        for(; cur != end && *cur == *start; cur++);

        /* Return the end of range. */
        return cur;
      }

      template <typename S, typename T>
      ANUBIS_INLINE std::pair<S,S> longestEqualRange(S start, S end, T value)
      {
        /* The start of the search. */

      }



//      /*********************************************************************//**
//       * Create a block of aligned memory. Since the C++11
//       * @param size
//       * @param align
//       * @return
//       */
//      ANUBIS_FORCE_INLINE static void * alignedAlloc(size_t size, size_t align)
//      {
//        /** TODO - When / if mingw implements the C++11 aligned_alloc()
//         * function, then remove this. */
//        #if ANUBIS_COMPILER == ANUBIS_COMPILER_MINGW
//            return __mingw_aligned_malloc(size, align);
//        #else
//          return aligned_alloc(align, size);
//        #endif
//      }

//      ANUBIS_FORCE_INLINE static void alignedFree(void * ptr)
//      {
//        __mingw_aligned_free(ptr);
//      }

      /*********************************************************************//**
       * Read a value from a byte buffer which is organised in big endian
       * byte order and translate it to the host's byte order.
       */
      template <typename T> ANUBIS_FORCE_INLINE static T fromBigEndian(
          const uint8_t * buffer, size_t buffLen, size_t index = 0)
      {
        /* The value to be returned. */
        T value;

        #ifdef ANUBIS_HOST_IS_LITTLE_ENDIAN
          for(size_t i = 0; i < sizeof(T); i++)
          {
            /* Shift the data by one byte. */
            value <<= 8;

            /* Or in the new value. */
            value |= buffer[index + i];
          }
        #endif /* ANUBIS_HOST_IS_LITTLE_ENDIAN */

        /* Return the value in the host byte order. */
        return value;
      }

      /***********************************************************************//**
       * Read a value of type T from the specified data which is in Little Endian
       * format.
       **************************************************************************/
      template <typename T> ANUBIS_FORCE_INLINE static T fromLittleEndian(
          const std::vector<uint8_t> & buffer, size_t index)
      {
        /* Make sure there is enough data left to read the value. */
        if(buffer.size() - index < sizeof(T))
        {
          ANUBIS_THROW_RUNTIME_EXCEPTION("Buffer underflow occured while"
            " attempting to read Little Endian value of size: " << sizeof(T) <<
            " bytes, from a buffer of size: " << buffer.size() << " bytes, at"
            " byte index: " << index << ".");
        }

        /* Check if the host is Little Endian. */
        #ifdef ANUBIS_HOST_IS_LITTLE_ENDIAN
          /* Read the data directly since the host has the same endianess. */
          return *reinterpret_cast<const T*>(buffer.data() + index);
        #else /* ANUBIS_HOST_IS_LITTLE_ENDIAN */


        #endif /* ANUBIS_HOST_IS_LITTLE_ENDIAN */
      }

      /*********************************************************************//**
       * Read a single byte at the index from the value specified.The function
       * is designed to act on integers in memory and consequently are not
       * bounded by endianess. Note that the LSB = Byte 0, and the
       * MSB = sizeof(T) - 1.
       *
       *
       ************************************************************************/
      template <typename T> static uint8_t getByte(size_t index, T value)
      {
        /* Make sure we are not trying to access a byte that is out of bounds.*/
        if(index >= sizeof(value))
        {
          ANUBIS_THROW_RUNTIME_EXCEPTION("Attempting to access an index that "
                                         "is out of bounds.");
        }

        /* Shift the value appropriately. */
        value >>= (index * 8);

        /* TODO - Should this be a reinterpret cast instead? */
        /* Mask and return only the last byte. */
        return static_cast<uint8_t>(0xFF & value);
      }
    };
  }
}


#endif /* ANUBIS_COMMON_MEMORY_HPP */
