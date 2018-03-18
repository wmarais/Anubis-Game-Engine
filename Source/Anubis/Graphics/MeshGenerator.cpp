#include "../../../Include/Anubis/Graphics/MeshGenerator.hpp"
#include "../../../Include/Anubis/Math/Vector4f.hpp"

using namespace Anubis::Math;
using namespace Anubis::Graphics;

/******************************************************************************/
std::shared_ptr<Mesh> MeshGenerator::createBox(
    float xSize, float ySize, float zSize,
    int xDivs, int yDivs, int zDivs,
    bool genNormals, bool genColours, bool genTexCoords)
{
//  /* Calculate the number of x-y vertices, faces and indices. */
//  size_t xyVertCount = (xDivs + 2) * (yDivs + 2);
//  size_t xyFaceCount = 2 * (xDivs + 1) * (yDivs + 1);
//  size_t xyIndexCount = 6 * (xDivs + 1) * (yDivs + 1);

//  /* Calculate the number of x-z vertices, faces and indices. */
//  size_t xzVertCount = (xDivs + 2) * (zDivs + 2);
//  size_t xzFaceCount = 2 * (xDivs + 1) * (zDivs + 1);
//  size_t xzIndexCount = 6 * (xDivs + 1) * (zDivs + 1);

//  /* Calculate the number of y-z vertices, faces and indices. */
//  size_t yzVertCount = (yDivs + 2) * (zDivs + 2);
//  size_t yzFaceCount = 2 * (xDivs + 1) * (zDivs + 1);
//  size_t yzIndexCount = 6 * (xDivs + 1) * (zDivs + 1);

//  /* Reserve enough space for all the mesh vertices. */
//  std::vector<Vector4f> vertices((2 * xyVertCount) +
//                                 (2 * xzVertCount) +
//                                 (2 * yzVertCount));

//  /* Reserve enough space for all the mesh indices. */
//  std::vector<uint16_t> indices((2 * xyIndexCount) +
//                                (2 * xzIndexCount) +
//                                (2 * yzIndexCount));

//  /* The array of normals. */
//  std::vector<Vector4f> normals;

//  /* Only reserve space if normals are generated. */
//  if(genNormals)
//  {
//    normals.reserve(vertices.size());
//  }

//  /* The array of colours. */
//  std::vector<Colour> colours;

//  /* Only reserver space if colours are generated. */
//  if(genColours)
//  {
//    colours.reserve(vertices.size());
//  }

//  /* The array of texture coordinates. */




//  /* The point closest to the camera. Since we are using a right handed
//   * coordinate system, the "smallest z" will be posisitive instead of
//   * negative like for the x and y axis. */
//  Vector4f minPoint = Vector4f::makePosition(-xSize / 2.0f,
//                                             -ySize / 2.0f,
//                                              zSize / 2.0f);

//  /* Calculate the furthest point. */
//  Vector4f maxPoint = Vector4f::makePosition( xSize / 2.0f,
//                                              ySize / 2.0f,
//                                             -zSize / 2.0f);

//  /* The step sizes along each axis. */
//  float xStep = xSize / static_cast<float>(xDivs + 1);
//  float yStep = ySize / static_cast<float>(yDivs + 1);
//  float zStep = zSize / static_cast<float>(zDivs + 1);

//  /* The current vertex insert index. */
//  size_t curVertIndex = 0;

//  /* The current indice insert index. */
//  size_t curIndiceIndex = 0;

//  /* Calculate the xy face for the box's front and back panel. */
//  float xOriginFront = -xSize / 2.0f;
//  float yOriginFront = -ySize / 2.0f;
//  float zOriginFront = zSize / 2.0f;

//  float xOriginBack = xSize / 2.0f;
//  float yOriginBack = -ySize / 2.0f;
//  float zOriginBack = -zSize / 2.0f;

//  /* Iterate through all the horizontal (y) slices. */
//  for(size_t curYDiv = 0; curYDiv < yDivs + 2; curYDiv++)
//  {
//    /* Iterate through all the vertical (x) slices. */
//    for(size_t curXDiv = 0; curXDiv < xDivs + 2; curXDiv++)
//    {
//      /* Create a vertex on the front panel's cross point. */
//      vertices.push_back(Vector4f::makePosition(
//                           xOriginFront + curXDiv * xStep,
//                           yOriginFront + curYDiv * yStep,
//                           zOriginFront));

//      /* Create a vertex on the back panel's cross point. */
//      vertices.push_back(Vector4f::makePosition(
//                           xOriginBack - curXDiv * xStep,
//                           yOriginBack + curYDiv * yStep,
//                           zOriginBack));
//    }
//  }


  /* Create a mesh to populate. */
  //std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
}
/******************************************************************************/
void MeshGenerator::genRectData(std::vector<uint16_t> & indices,
  std::vector<Math::Vector4f> & vertices, std::vector<Math::Vector4f> & normals,
  std::vector<Math::Vector2f> & texCoords, std::vector<Colour> & colours,
  float mSize, float nSize, size_t mDivs, size_t nDivs,
  const Math::Vector4f & mAxis, const Math::Vector4f & nAxis,
  const Math::Vector4f & origin, bool genNormals, bool genTexCoords,
  bool genColours)
{
  /* Calculate the step vectors. */
  Vector4f mStep = mAxis * mSize / static_cast<float>(mDivs + 1);
  Vector4f nStep = nAxis * nSize / static_cast<float>(nDivs + 1);

  /* Calculate the normal for the plane. */
  Vector4f normal = mAxis.cross(nAxis);
  normal.normalise();

  /* Calculate the colour to use. */
  Colour colour(1.0f, 1.0f, 1.0f, 1.0f);

  /* Store the first indice location. */
  size_t curIndice = vertices.size();

  /* Calculate the number of triangle faces. */
  size_t faceCount = 2 * (mDivs + 1) * (nDivs + 1);

  /* Count out all the n steps. The +2 is to account for the vertices on the
   * border of the n axis. */
  for(size_t n = 0; n < (nDivs + 2); n++)
  {
    /* Calculate the current starting position. */
    Vector4f curVert = origin + nStep * static_cast<float>(n);

    /* Count of all the m steps. The +2 is to account for the vertices on the
     * border of the m axis. */
    for(size_t m = 0; m < (mDivs + 2); m++)
    {
      /* Store the current vertex in the vertices array. */
      vertices.push_back(curVert);

      /* Check if a normal should be added. */
      if(genNormals)
      {
        normals.push_back(normal);
      }

      /* Check if colours should be added. */
      if(genColours)
      {
        colours.push_back(colour);
      }

      /* Check if the texture coordinates should be calculated. */
      if(genTexCoords)
      {
        /* Calculate the vector formed between the current vertex and the
         * origin point. */
        Vector4f diffVec = curVert - origin;

        /* Project the vector onto the m axis. */
        float u = diffVec.dot(mAxis) / mSize;

        /* Project the vector onto the n axis. */
        float v = diffVec.dot(nAxis) / nSize;

        /* Add the texture coordinate. */
        texCoords.push_back(Vector2f(u, v));
      }

      /* Calculate the next vertex. */
      curVert += mStep;
    }
  }

  /* Calculate all the indices for the faces. */
  for(size_t f = 0; f < faceCount; f++)
  {
    /* The first face. */
    indices.push_back(curIndice);
    indices.push_back(curIndice + 1);
    indices.push_back(curIndice + mDivs + 3);

    /* The second face. */
    indices.push_back(curIndice + mDivs + 3);
    indices.push_back(curIndice + mDivs + 2);
    indices.push_back(curIndice);

    /* Proceed to the next indice. */
    ++curIndice;
  }
}

/******************************************************************************/
std::shared_ptr<Mesh> MeshGenerator::createRectangle(float mSize, float nSize,
  size_t mDivs, size_t nDivs, const Math::Vector4f & mAxis,
  const Math::Vector4f & nAxis, const Math::Vector4f & origin,
  bool genNormals, bool genTexCoords, bool genColours)
{
  /* Calculate the number of indices. */
  size_t indicesCount = 6 * (mDivs + 1) * (nDivs + 1);

  /* Calculate the number of vertices. */
  size_t verticesCount = (mDivs + 2) * (nDivs + 2);


  /* Create the indices list and reserver memory for all the idnices. */
  std::vector<uint16_t> indices;
  indices.reserve(indicesCount);

  /* Create the vertices array and reserver memory for all vertices. */
  std::vector<Math::Vector4f> vertices;
  vertices.reserve(verticesCount);

  /* Create the normals array and only reserver memory if normals must be
   * generated. */
  std::vector<Math::Vector4f> normals;
  if(genNormals)
  {
    normals.reserve(verticesCount);
  }

  /* Create the texture coordinate array and only reserve memory if texture
   * coordinates must be generated. */
  std::vector<Math::Vector2f> texCoords;
  if(genTexCoords)
  {
    texCoords.reserve(verticesCount);
  }

  /* Create the colours array and only reserve memory if colours must be
   * generated. */
  std::vector<Colour> colours;
  if(genColours)
  {
    colours.reserve(verticesCount);
  }

  /* Generate the data. */
  genRectData(indices, vertices, normals, texCoords, colours, mSize, nSize,
              mDivs, nDivs, mAxis, nAxis, origin, genNormals, genTexCoords,
              genColours);

  /* The attribute arrays of the mesh. */
  std::vector<AttributeArray> attribs;

  /* Create the attribute arrays. */
  attribs.push_back(AttributeArray("Vertices", vertices));

  if(genNormals)
  {
    attribs.push_back(AttributeArray("Normals", normals));
  }

  if(genTexCoords)
  {
    attribs.push_back(AttributeArray("TexCoords", texCoords));
  }

  if(genColours)
  {
    attribs.push_back(AttributeArray("Colours", colours));
  }

  /* Interleave the vertices. */
  MixedAttributeArray mixedAttribs(attribs);

  /* Create and return the mesh. */
  return std::make_shared<Mesh>(indices, mixedAttribs);
}



