#ifndef ANUBIS_COMMON_SUB_OBJ_HPP
#define ANUBIS_COMMON_SUB_OBJ_HPP

#include "IdentObj.hpp"

namespace Anubis
{
  namespace Common
  {
    /***********************************************************************//**
     * A class that provide the basic interfaces for identifying the parent
     * object of this sub object.
     **************************************************************************/
    class SubObj : public IdentObj
    {
      /** The UUID of the component that this sub component belongs too. */
      UUID fCompUUID;

    public:
      /*********************************************************************//**
       * Create a new sub component object with a random uuid.
       ************************************************************************/
      ANUBIS_INLINE SubObj() {}

      /*********************************************************************//**
       * Create a new sub component object with the specific uuid.
       *
       * @param uuid  The UUID of the object.
       ************************************************************************/
      ANUBIS_INLINE SubObj(const UUID & uuid) : fCompUUID(uuid) {}

      /*********************************************************************//**
       * An empty virtual destructor to ensure that the subclass' destructor
       * is invoked.
       ************************************************************************/
      ANUBIS_INLINE virtual ~SubObj() {}

      /*********************************************************************//**
       * Return the uuid of the component that this sub component belongs too.
       *
       * @return  The uuid of the component this sub component belongs too.
       ************************************************************************/
      ANUBIS_INLINE const UUID & getCompID() const noexcept
      {
        return fCompUUID;
      }

      /*********************************************************************//**
       * Set the uuid of the component that this sub component belongs too.
       *
       * @param uuid  The uuid of the component that this one belongs too.
       ************************************************************************/
      ANUBIS_INLINE void setCompID(const UUID & uuid)
      {
        fCompUUID = uuid;
      }
    };
  }
}

#endif /* ANUBIS_COMMON_SUB_OBJ_HPP */
