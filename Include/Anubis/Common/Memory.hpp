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

      ANUBIS_FORCE_INLINE static void * alignedAlloc(size_t size, size_t align)
      {
        return __mingw_aligned_malloc(size, align);
      }

      ANUBIS_FORCE_INLINE static void alignedFree(void * ptr)
      {
        __mingw_aligned_free(ptr);
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
