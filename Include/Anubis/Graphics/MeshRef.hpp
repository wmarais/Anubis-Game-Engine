#ifndef ANUBIS_GRAPHICS_MESH_REF_HPP
#define ANUBIS_GRAPHICS_MESH_REF_HPP

#include "../Common/Misc.hpp"
#include "Camera.hpp"

#include "BufferObject.hpp"

#include "../Common/UUID.hpp"

namespace Anubis
{
  namespace Graphics
  {
    class MeshRef final
    {
      /** The UUID of the object. */
      Common::UUID fUUID;

    public:

      MeshRef() {}
      MeshRef(const Common::UUID & uuid) : fUUID(uuid) {}


      /*********************************************************************//**
       * Prepare the mesh for rendering using the specified camera.
       *
       * @param camera  The camera that will be used to render the mesh.
       ************************************************************************/
      void activate(std::shared_ptr<Camera> camera) {}

      void render(const Camera * camera) {}

      void render(const Camera * camera, const std::vector<uint16_t> & indices) {}

      /*********************************************************************//**
       * Compare the BufferObject of this mesh with the rhs mesh and check which
       * pointer value is less than the other. This is used to sort the meshes
       * by BufferObject reducing the number of buffer changes that has to be
       * made during rendering. (See it's usage in RenderingQueue.)
       *
       * @param rhs The MesRef on the rhs side of the comparison.
       * @return    True if the pointer of this object's BufferObject is less
       *            than the rhs object's BufferObject, else False.
       ************************************************************************/
      bool operator < (const MeshRef * rhs) { return true; }
    };
  }
}

#endif /* ANUBIS_GRAPHICS_MESH_REF_HPP */
