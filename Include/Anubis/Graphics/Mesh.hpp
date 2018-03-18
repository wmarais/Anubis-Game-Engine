#ifndef ANUBIS_GRAPHICS_MESH_HPP
#define ANUBIS_GRAPHICS_MESH_HPP


#include "../Common/UUID.hpp"
#include "MixedAttributeArray.hpp"

namespace Anubis
{
  namespace Graphics
  {
    class BufferObject;

    /***********************************************************************//**
     * A class tha contains all the renderable information of a single mesh.
     * Always use shared_ptr()s to store instances of meshes since they can be
     * referred to by many objects at the same time.
     *
     **************************************************************************/
    class Mesh
    {
      /* The BufferObject class is made a friend of the mesh. This allows the
       * interface of the Mesh to be restricted such that user can't overwritte
       * the associated BufferObject etc. */
      friend class BufferObject;

      /** The indexes of the faces in the mesh. */
      std::vector<uint16_t> fIndexes;

      /** The attribute arrays associated with this mesh. */
      std::vector<std::shared_ptr<MixedAttributeArray>> fMixedAttribArrays;

      /** The buffer object used to render the mesh. */
      std::shared_ptr<BufferObject> fBufferObject;

    public:
      Mesh(const std::vector<uint16_t> & indexes,
           MixedAttributeArray & mixedAttribArrays) {}

      Mesh(const std::vector<uint16_t> & indexes,
           const std::vector<MixedAttributeArray> & mixedAttribArrays);

      Mesh(std::vector<uint16_t> indexes,
           std::initializer_list<MixedAttributeArray> attribArrays);

      //Mesh(std::vector<uint16_t> indices, std::vector<MixedAttributeArray>

      /*********************************************************************//**
       * @brief mixedAttribArrays
       * @return
       ************************************************************************/
      const std::vector<std::shared_ptr<MixedAttributeArray>> &
        mixedAttribArrays() const;

//      /*********************************************************************//**
//       * Compare the attributes layout of this mesh to another mesh. Buffer
//       * Objects require that the associated meshes have:
//       *
//       *  - The same number of Mixed Attribute Arrays.
//       *  - Each Mixed Attribute Array has the same layout, that is:
//       *    o Same Names
//       *    o Same Types
//       *
//       * @param rhs The mesh to compare this mesh too.
//       * @return    True if the mesh satisfy the above criteria for equality,
//       *            otherwise, false.
//       ************************************************************************/
//      bool matchesLayout(const Mesh & rhs);

      /*********************************************************************//**
       * The amount of memory space required to store all the mixed attribute
       * arrays in a buffer object.
       *
       * @return
       ************************************************************************/
      size_t bufferArraySize() const;

      /*********************************************************************//**
       * @brief render
       ************************************************************************/
      void render() {}

      /*********************************************************************//**
       * Set the buffer object that is used for rendering. This method allows
       * the buffer object to be cached instead of searched for during
       * rendering. <b>It is mandatory for this function to be called, other
       * wise the mesh will not (can not) be rendered.</b>
       *
       * @param buffObj The buffer object to use for rendering.
       ************************************************************************/
      void setBufferObject(std::shared_ptr<BufferObject> buffObj)
      {
        /* Set the required buffer object. */
        fBufferObject = buffObj;
      }

      /*********************************************************************//**
       * Release the BufferObject reference used for rendering. This allows
       * the BufferObject's reference count to reduce and the object to
       * eventually be destroyed when the count reaches 0.
       ************************************************************************/
      void releaseBufferObject()
      {
        /* Release the reference to the buffer object. */
        fBufferObject = nullptr;
      }


    };
  }
}

#endif /* ANUBIS_GRAPHICS_MESH_HPP */
