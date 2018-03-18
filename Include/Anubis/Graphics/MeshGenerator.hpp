#ifndef ANUBIS_GRAPHICS_MESH_GENERATOR_HPP
#define ANUBIS_GRAPHICS_MESH_GENERATOR_HPP

#include "Mesh.hpp"
#include "../Math/Vector4f.hpp"

namespace Anubis
{
  namespace Graphics
  {
    class MeshGenerator
    {
      /*********************************************************************//**
       * Generate the raw data for a rectangle mesh. This function has been
       * created to reduce the duplicate effort since the box mesh generator
       * also use it to generate all six sides of the box.
       *
       * For more information on how this function works, please refer to the
       * Anubis Cookbook's Mesh Generators - Rectangle Generator section.
       *
       * @param indices       The indices array forming the faces of the mesh.
       * @param vertices      The vertices of the mesh.
       * @param normals       The per-vertex normals of the mesh.
       * @param texCoords     The per-vertex 2D texture coordinates of the mesh.
       * @param colours       The per-vertex colours of the mesh.
       * @param mSize         The size of the rectangle along the m axis.
       * @param nSize         The size of the rectangle along the n axis.
       * @param mDivs         The number of m axis divisions / steps.
       * @param nDivs         The number of n axis divisions / steps.
       * @param mAxis         The direction vector of the m axis. This must be
       *                      a unit vector.
       * @param nAxis         The direction vector of the n axis. This must be
       *                      a unit vector.
       * @param origin        The starting vertex of the rectangle.
       * @param genNormals    Indicate whether normals should be generated.
       * @param genTexCoords  Indicate whether texture coordinates should be
       *                      generated.
       * @param genColours    Indicate whether colours should be generated.
       ************************************************************************/
      static void genRectData(std::vector<uint16_t> & indices,
                              std::vector<Math::Vector4f> & vertices,
                              std::vector<Math::Vector4f> & normals,
                              std::vector<Math::Vector2f> & texCoords,
                              std::vector<Colour> & colours,
                              float mSize, float nSize,
                              size_t mDivs, size_t nDivs,
                              const Math::Vector4f & mAxis,
                              const Math::Vector4f & nAxis,
                              const Math::Vector4f & origin,
                              bool genNormals = true,
                              bool genTexCoords = false,
                              bool genColours = false);

    public:
      /*********************************************************************//**
       * Create a rectangle mesh. The rectangle can be created in arbritary
       * directions.
       *
       * For more information on how this function works, please refer to the
       * Anubis Cookbook's Mesh Generators - Rectangle Generator section.
       *
       * @param mSize         The size of the rectangle along the m axis.
       * @param nSize         The size of the rectangle along the n axis.
       * @param mDivs         The number of steps along the m axis.
       * @param nDivs         The number of steps along the n axis.
       * @param mAxis         The direction vector of the m axis. This must be
       *                      a unit vector.
       * @param nAxis         The direction vector of the n axis. This must be
       *                      a unit vector.
       * @param origin        The first vertex in the mesh / starting corner of
       *                      the rectangle.
       * @param genNormals    Indicate if normals must be generated.
       * @param genTexCoords  Indicate if texture coordinates must be generated.
       * @param genColours    Indicate if colours must be generated.
       * @return              The generated mesh object.
       ************************************************************************/
      static std::shared_ptr<Mesh> createRectangle(float mSize, float nSize,
                                                  size_t mDivs, size_t nDivs,
                                                  const Math::Vector4f & mAxis,
                                                  const Math::Vector4f & nAxis,
                                                  const Math::Vector4f & origin,
                                                  bool genNormals = true,
                                                  bool genTexCoords = false,
                                                  bool genColours = false);




      static std::shared_ptr<Mesh> createBox(float xSize, float ySize, float zSize,
          int xDivs, int yDivs, int zDivs,
          bool genNormals, bool genColours, bool genTexCoords);

    };
  }
}

#endif /* ANUBIS_GRAPHICS_MESH_GENERATOR_HPP */
