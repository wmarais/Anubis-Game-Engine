#include "../../../Include/Anubis/Common/System.hpp"
#include "../../../Include/Anubis/Graphics/BufferObject.hpp"

using namespace Anubis::Graphics;

/******************************************************************************/
BufferObject::BufferObject(const std::vector<std::shared_ptr<Mesh>> & meshes)
{

}

/******************************************************************************/
BufferObject::BufferObject(std::initializer_list<std::shared_ptr<Mesh>> meshes)
{

//  /* Calculate all the memory required to store the meshes in Buffer Array. */
//  size_t arrayBufferLen = 0;

//  for(auto mesh : meshes)
//  {
//    arrayBuffLen +=
//  }





//  /* The lenght of the buffer object. */
//  size_t bufferLength = 0;

//  /* Reset space for the buffer array ids. */
//  fArrayBuffIDs.resize(meshes.size());




//  /* Generate the required buffers. */
//  glGenBuffers(fArrayBuffIDs.size(), fArrayBuffIDs.data());

//  /* Calculating the length of buffer. */
//  for(auto mesh : meshes)
//  {
//    bufferLength += mesh->
//  }

//  glGenBuffers(1, &fID);
//  glBindBuffer(GL_ARRAY_BUFFER, fID);

//  /* Load the data of the meshes. */

}

/******************************************************************************/
void BufferObject::releaseMeshes()
{
  /* Release all the associated meshes. */

}
