#ifndef ANUBIS_COMMON_UUID_HPP
#define ANUBIS_COMMON_UUID_HPP

#include "Misc.hpp"

namespace Anubis
{
  namespace Common
  {
    /***********************************************************************//**
     * A UUID class used for uniquely identifying assets, objects, players,
     * etc. All the standard binary comparison function has been implemneted
     * such that the UUIDs can be used for fast binary sorts and lookups. I.e.
     * if assets are stored in a vector and sorted by UUID, then a binary
     * search can be used to find the respective item.
     **************************************************************************/
    class UUID final
    {
      /** The number of octets used to decribe a UUID. */
      static constexpr const size_t kOctetCount = 16;

      /** The octets representing the UUID. */
      uint8_t fOctets[kOctetCount];

    public:

      /*********************************************************************//**
       * Create a new UUID from scratch.
       ************************************************************************/
      UUID();

      /*********************************************************************//**
       * Comparison operator to check if two UUIDs match.
       *
       * @param rhs The other UUID to compare too.
       * @return    True if this UUID matches the rhs UUID, else false.
       ************************************************************************/
      ANUBIS_FORCE_INLINE bool operator == (const UUID & rhs) const noexcept
      {
        /* Compare the two UUID's octets for equality and return ther result. */
        return memcmp(fOctets, rhs.fOctets, kOctetCount) == 0;
      }

      /*********************************************************************//**
       * Comparison operator to check if two UUIDs don't match.
       *
       * @param rhs The other UUID to compare too.
       * @return    True if this UUID don't match the rhs UUID, else false.
       ************************************************************************/
      ANUBIS_FORCE_INLINE bool operator != (const UUID & rhs) const noexcept
      {
        /* Compare the two UUID's octets for equality and return ther result. */
        return memcmp(fOctets, rhs.fOctets, kOctetCount) != 0;
      }

      /*********************************************************************//**
       * Compare this UUID to the rhs UUID and return true if this UUID is
       * less than the rhs UUID.
       *
       * @param rhs The UUID on the rhs of the expression.
       * @return    True if this UUID is smaller then the rhs UUID, else false.
       ************************************************************************/
      ANUBIS_FORCE_INLINE bool operator < (const UUID & rhs) const noexcept
      {
        return memcmp(fOctets, rhs.fOctets, kOctetCount) < 0;
      }

      /*********************************************************************//**
       * Compare this UUID to the rhs UUID and return true if this UUID is
       * less than or equal to the rhs UUID.
       *
       * @param rhs The UUID on the rhs of the expression.
       * @return    True if this UUID is less than or equal to the rhs UUID,
       *            else false.
       ************************************************************************/
      ANUBIS_FORCE_INLINE bool operator <= (const UUID & rhs) const noexcept
      {
        /* Get the result of the comparison. */
        int result = memcmp(fOctets, rhs.fOctets, kOctetCount);

        /* Evaluate the result. */
        return result <= 0;
      }

      /*********************************************************************//**
       * Compare this UUID to the rhs UUID and return true if this UUID is
       * greater then the rhs UUID.
       *
       * @param rhs The UUID on the rhs of the expression.
       * @return    True if this UUID is greater then the rhs UUID, else false.
       ************************************************************************/
      ANUBIS_FORCE_INLINE bool operator > (const UUID & rhs) const noexcept
      {
        return memcmp(fOctets, rhs.fOctets, kOctetCount) > 0;
      }

      /*********************************************************************//**
       * Compare this UUID to the rhs UUID and return true if this UUID is
       * greater than or equal to then the rhs UUID.
       *
       * @param rhs The UUID on the rhs of the expression.
       * @return    True if this UUID is greater than or equal to the rhs UUID,
       *            else false.
       ************************************************************************/
      ANUBIS_FORCE_INLINE bool operator >= (const UUID & rhs) const noexcept
      {
        /* Get the result of the comparison. */
        int result = memcmp(fOctets, rhs.fOctets, kOctetCount);

        /* Evaluate ther esult. */
        return result >= 0;
      }

      /*********************************************************************//**
       * Write the UUID out as a string to the specified output stream.
       *
       * @param os  The output stream to write too.
       ************************************************************************/
      void writeString(std::ostream & os) const;

      /*********************************************************************//**
       * Write the UUID out as a binary string to the specified output stream.
       *
       * @param os  The output stream to write too.
       ************************************************************************/
      void writeBinary(std::ostream & os) const;
    };
  }
}

#endif /* ANUBIS_COMMON_UUID_HPP */
