#ifndef ANUBIS_GRAPHICS_LOD_SET_HPP
#define ANUBIS_GRAPHICS_LOD_SET_HPP

#include "Mesh.hpp"

namespace Anubis
{
  namespace Graphics
  {
    /***********************************************************************//**
     * Create a set of meshes that allow
     **************************************************************************/
    class LODSet
    {
    public:

      struct Level
      {
        /** The distance from the camera before this mesh is used. */
        float fRange;

        /** The mesh to use for rendering when for the rendering range of
         * this object. */
        std::shared_ptr<Mesh> fMesh;

        /* Create a range level with the specific range and mesh mapping. */
        Level(float range, std::shared_ptr<Mesh> mesh) : fRange(range),
          fMesh(mesh) {}
      };

    protected:

      /** The UUID of the object. */
      Common::UUID fUUID;

      /* Various level of details of the mesh. */
      std::vector<Level> fLevels;

      /** Indicate whether the LOD set has been sorted. */
      bool fSorted;

      ANUBIS_INLINE void sort()
      {
        /* Sort the array by ascending range. */
        std::sort(fLevels.begin(), fLevels.end(),
        [](const Level & lhs, const Level & rhs) -> bool
        {
          return lhs.fRange < rhs.fRange;
        });

        /* Mark the set as sorted. */
        fSorted = true;
      }

    public:

      /*********************************************************************//**
       * Create the object providing atleast one level of detail to satisfy the
       * requirements for the calcIndex() function.
       *
       * @param level
       ************************************************************************/
      ANUBIS_INLINE LODSet(const Level & level) : fSorted(true)
      {
        /* Add the level to the set. */
        fLevels.push_back(level);
      }

      /*********************************************************************//**
       * Create the LODSet with one or more level and sort them in ascending
       * range.
       *
       * @param levels  The LOD levels to add the to set.
       ************************************************************************/
      ANUBIS_INLINE LODSet(const std::vector<Level> & levels) : fSorted(true)
      {
        /* Ensure there is at least one level provided. */
        if(levels.size() == 0)
        {
          ANUBIS_THROW_RUNTIME_EXCEPTION("No LOD Levels have been provided "
            "to the constructor of the LODSet object.");
        }

        /* Add the levels to the set. */
        fLevels = levels;

        /* Sort the array by ascending range. */
        sort();
      }

      /*********************************************************************//**
       * Calculate the index of the mesh to use for rendering when the mesh
       * is "distance" away from the camera. This function allows the index to
       * be cached avoiding more complex / costly repeated calculations.
       *
       * @param distance  The distance that the camera is from the mesh.
       * @return          The index of the mesh to use for rendering.
       ************************************************************************/
      ANUBIS_INLINE size_t calcIndex(float distance)
      {
        /* Check if the set has been sorted. */
        if(!fSorted)
        {
          /* Sort the array. */
          sort();
        }

        /* The index to use for rendering. */
        size_t index = 0;

        /* Since the ranges are sorted from shortest to furthest, we can simply
         * iterate through the loop until the appropriate range is found.
         * Furthermore, if we iterate upto size() - 1, then we can also
         * guarentee there will always be a index. Generally:
         *
         *  if size() == n, and for all n fLevels the distance > fRange,
         *    then index == n - 1
         *
         * Thus the index will always be valid as long as size() > 0. */
        for(index = 0; index < fLevels.size() - 1; index++)
        {
          if(distance < fLevels[index].fRange)
          {
            /* This is the appropriate range. */
            break;
          }
        }

        /* Return the index. */
        return index;
      }
    };
  }
}


#endif /* ANUBIS_GRAPHICS_LOD_SET_HPP */
