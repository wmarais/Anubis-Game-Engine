#include "../../../Include/Anubis/Graphics/Texture2D.hpp"
#include "../../../Include/Anubis/Common/System.hpp"

using namespace Anubis::Graphics;

/******************************************************************************/
Texture2D::Texture2D(const PixelMap & pixelMap, bool genMipMaps) :
  Texture(GL_TEXTURE_2D, genMipMaps), fWidth(pixelMap.width()),
  fHeight(pixelMap.height()), fPixelType(pixelMap.pixelType())
{
  /* Indicate whether the texture has mipmaps. */
  fHasMipMaps = genMipMaps;

  /* Load the texture data. */
  glTexImage2D(fTarget,
               0,
               kFormats[static_cast<uint8_t>(fPixelType)],
               fWidth,
               fHeight,
               0,
               kFormats[static_cast<uint8_t>(fPixelType)],
               GL_UNSIGNED_BYTE,
               pixelMap.data());

  /* Check if mip maps must be generated. */
  if(fHasMipMaps)
  {
    /* Generate the mip maps. */
    glGenerateMipmap(GL_TEXTURE_2D);
  }
}

/******************************************************************************/
Texture2D::~Texture2D()
{
  /* Nothing to do here, but this must exist for std::unique_ptr<> to work with
   * the forward declared Data struct. */
}

/******************************************************************************/
void Texture2D::setData(const PixelMap &pixelMap)
{
  /* Check if there is a resolution missmatch. */
  if(pixelMap.width() != fWidth || pixelMap.height() != fHeight ||
     pixelMap.pixelType() != fPixelType)
  {
    /* Record the new resultion. */
    fWidth = pixelMap.width();
    fHeight = pixelMap.height();
    fPixelType = pixelMap.pixelType();

    /* Recreate the texture object and load the texture data. */
    glTexImage2D(fTarget, 0,
                 kFormats[static_cast<uint8_t>(fPixelType)],
                 fWidth, fHeight, 0,
                 kFormats[static_cast<uint8_t>(fPixelType)],
                 GL_UNSIGNED_BYTE, pixelMap.data());

  }
  else
  {
    /* Otherwise just update the pixels. */
    glTexSubImage2D(fTarget, 0, 0, 0, fWidth, fHeight,
                    kFormats[static_cast<uint8_t>(fPixelType)],
                    GL_UNSIGNED_BYTE, pixelMap.data());
  }
}

