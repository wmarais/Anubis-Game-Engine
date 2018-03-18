#ifndef ANUBIS_GRAPHICS_BUFFER_OBJECT_HPP
#define ANUBIS_GRAPHICS_BUFFER_OBJECT_HPP

#include "Mesh.hpp"

namespace Anubis
{
  namespace Graphics
  {
    /***********************************************************************//**
     * A wrapper around the OpenGL buffer object mechanism used specifically
     * for meshes.
     **************************************************************************/
    class BufferObject
    {
      /* The ARRAY_BUFFER IDs. */
      std::vector<uint32_t> fArrayBuffIDs;

      /* The ELEMENT_ARRAY_BUFFER ID used for the index arrays. */
      uint32_t fIndexID;

      /* The list of meshes associated with this buffer object. */
      std::vector<std::shared_ptr<Mesh>> fMeshes;

    public:

      BufferObject(const std::vector<std::shared_ptr<Mesh>> & meshes);
      BufferObject(std::initializer_list<std::shared_ptr<Mesh>> meshes);


      /*********************************************************************//**
       * Release the meshes associated with the buffer object. This must be used
       * to release the reference to the meshes in order for the the reference
       * counters to count down to 0 and delete the mesh objects.
       ************************************************************************/
      void releaseMeshes();




    };
  }
}

#endif /* ANUBIS_GRAPHICS_BUFFER_OBJECT_HPP */
