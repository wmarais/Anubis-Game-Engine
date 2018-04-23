#include "../../../Include/Anubis/Common/System.hpp"
#include "../../../Include/Anubis/Graphics/BufferObject.hpp"

using namespace Anubis::Graphics;

/******************************************************************************/
const uint32_t BufferObject::kGLModes[BufferObject::kModesCount] =
{
  GL_STATIC_DRAW,
  GL_DYNAMIC_DRAW,
  GL_STREAM_DRAW
};

/******************************************************************************/
BufferObject::BufferObject(const std::vector<std::shared_ptr<Mesh>> & meshes,
                           Modes mode) : fID(0)
{
  /* Creat the buffer object. */
  //glGenBuffers(1, fID);


}

/******************************************************************************/
void BufferObject::releaseMeshes()
{
  /* Release all the associated meshes. */

}
