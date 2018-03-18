#ifndef ANUBIS_COMMON_IDENT_OBJ_HPP
#define ANUBIS_COMMON_IDENT_OBJ_HPP

#include "UUID.hpp"

namespace Anubis
{
  namespace Common
  {
    /***********************************************************************//**
     * A class that provide the basic interfaces for an identifiable object. All
     * identifiable objects has a uniqueue identifier (uuid).
     **************************************************************************/
    class IdentObj
    {
      /* The UUID of the object. */
      UUID fUUID;

    public:

      /*********************************************************************//**
       * Create a new identifiable object with a random uuid.
       ************************************************************************/
      ANUBIS_INLINE IdentObj() {}

      /*********************************************************************//**
       * Create a new Identifiable object with the specific uuid.
       *
       * @param uuid  The UUID of the object.
       ************************************************************************/
      ANUBIS_INLINE IdentObj(const UUID & uuid) : fUUID(uuid) {}

      /*********************************************************************//**
       * An empty virtual destructor to ensure that the subclass' destructor
       * is invoked.
       ************************************************************************/
      ANUBIS_INLINE virtual ~IdentObj() {}

      /*********************************************************************//**
       * Return the UUID of the object.
       *
       * @return  The UUID of the object.
       ************************************************************************/
      ANUBIS_INLINE const UUID & getID() const noexcept
      {
        return fUUID;
      }

      /*********************************************************************//**
       * Set the UUID of the object.
       *
       * @param uuid  The new object UUID.
       ************************************************************************/
      ANUBIS_INLINE void setID(const UUID & uuid) noexcept
      {
        fUUID = uuid;
      }
    };
  }
}

#endif /* ANUBIS_COMMON_IDENT_OBJ_HPP */
