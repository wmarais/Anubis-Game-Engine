#ifndef ANUBIS_PHYSICS_COLLISION_VOLUME_HPP
#define ANUBIS_PHYSICS_COLLISION_VOLUME_HPP

#include "../Common/UUID.hpp"
#include "../Math/Ray.hpp"

namespace Anubis
{
  namespace Physics
  {
    /***********************************************************************//**
     * The high level abstraction for bounding volumes.
     **************************************************************************/
    class BoundingVolume
    {
    protected:
      /** The diffirent bounding volume types. */
      enum class Types
      {
        /** The volume is a bounding box. */
        Box,

        /** The volume is a bounding sphere. */
        Sphere,

        /** The volume is a bounding mesh. */
        Mesh
      };

      /** The UUID of the bounding volume. */
      Common::UUID fUUID;

      /** The current volume type. */
      Types fType;

    public:

      /*********************************************************************//**
       * Initialise a bounding box of the specific type.
       *
       * @param type  The bounding box type.
       ************************************************************************/
      ANUBIS_FORCE_INLINE BoundingVolume(Types type) : fType(type) {}

      /*********************************************************************//**
       * Initialise the bounding volume with the specific type and UUID.
       *
       * @param type  The bounding volume type.
       * @param uuid  The UUID of the bounding volume.
       ************************************************************************/
      ANUBIS_FORCE_INLINE BoundingVolume(Types type, const Common::UUID & uuid)
        : fUUID(uuid), fType(type) {}

      /*********************************************************************//**
       * A blank virtual destructor to ensure that the destructor of any
       * subclass is invoked upon destruction.
       ************************************************************************/
      ANUBIS_FORCE_INLINE virtual ~BoundingVolume() {}

      /*********************************************************************//**
       * Return a constant reference to the UUID of the bounding volume. The
       * UUID is used to uniquely identify this item in the collision volume
       * library.
       *
       * @return  The uuid of the bounding volume.
       ************************************************************************/
      ANUBIS_FORCE_INLINE const Common::UUID & uuid() const
      {
        return fUUID;
      }

      /*********************************************************************//**
       * Set the UUID of the bounding volume.
       *
       * @param uuid  The new UUID of the bounding volume.
       ************************************************************************/
      ANUBIS_FORCE_INLINE void setUUID(const Common::UUID & uuid)
      {
        fUUID = uuid;
      }

      /*********************************************************************//**
       * Calculate whether the specified ray intersects the bounding volume or
       * not. Return the list of distances corresponding to the intersections.
       *
       * @param ray       The ray to check for intersections.
       * @param distance  The distances along the ray that the intersection(s)
       *                  occured.
       ************************************************************************/
      virtual void intersect(const Math::Ray & ray,
                             std::vector<float> & distances) = 0;
    };
  }
}

#endif /* ANUBIS_PHYSICS_COLLISION_VOLUME_HPP */
