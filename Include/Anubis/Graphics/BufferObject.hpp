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
    class BufferObject final
    {
      /** The number of diffirent buffer object types. */
      static const size_t kModesCount = 3;

      /** The lookup table matching Types to OpenGL Types. */
      static const uint32_t kGLModes[kModesCount];

      /** The ID of the OpenGL buffer object. */
      uint32_t fID;

    public:


      enum Modes
      {
        /** Use to indicate that the buffer will rarely change. */
        Static  = 0,

        /** Used to indicate that the buffer will change frequently, but likely
         * not every frame. */
        Dynamic = 1,

        /** Use to indicate that the buffer change every frame. */
        Stream  = 2
      };


      BufferObject(const std::vector<std::shared_ptr<Mesh>> & meshes,
                   Modes mode);

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
