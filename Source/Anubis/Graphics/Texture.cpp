#include "../../../Include/Anubis/Graphics/Texture.hpp"
#include "../../../Include/Anubis/Common/System.hpp"

using namespace Anubis::Graphics;

/******************************************************************************/
uint32_t Texture::fActiveTexUnit = 0;

/******************************************************************************/
const int32_t Texture::kFormats[Texture::kFormatsCount] =
{
  GL_RED,
  GL_RGB,
  GL_RGBA,
  GL_BGR,
  GL_BGRA
};

/******************************************************************************/
const int32_t Texture::kGLWrapModes[Texture::kWrapModesCount] =
{
  GL_REPEAT,
  GL_MIRRORED_REPEAT,
  GL_CLAMP_TO_EDGE,
  GL_CLAMP_TO_BORDER
};


/******************************************************************************/
Texture::Texture(uint32_t target, bool hasMipMaps) : fID(0), fTarget(target),
  fHasMipMaps(hasMipMaps)
{
  /* Generate the texture object. */
  glGenTextures(1, &fID);

  /* Bind the texture. */
  bind();
}

/******************************************************************************/
Texture::~Texture()
{
  /* Check if the texture object was created. */
  if(fID != 0)
  {
    /* Delete the texture. */
    glDeleteTextures(1, &fID);
  }
}

/******************************************************************************/
void Texture::setFilter(Filters filter, uint32_t aniLevel)
{
  /* Check the type of filter to apply. */
  switch(filter)
  {
    /* Setup Nearest Neighbour Filtering. */
    case Filters::Nearest:
      /* Set the magnification filter. */
      glTexParameteri(fTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      /* Set the minification filter. */
      glTexParameteri(fTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    break;

    /* Setup Bilinear Filtering. */
    case Filters::Bilinear:
      /* Set the magnification filter. */
      glTexParameteri(fTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      /* Set the minification filter. */
      glTexParameteri(fTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    break;

    /* Setup Trilinear Filtering. */
    case Filters::Trilinear:
      /* Set the magnification filter. */
      glTexParameteri(fTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      /* Set the minification filter. */
      glTexParameteri(fTarget, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
    break;

    /* Setup Anisotropic Filtering. */
    case Filters::Anisotropic:
      /* Set the magnification filter. */
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      /* Set the minification filter. */
      glTexParameteri(fTarget, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);

      /* Set the anisotropy level. */
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniLevel);
    break;
  }
}

/******************************************************************************/
void Texture::setWrap(WrapModes sWrap, WrapModes tWrap, WrapModes rWrap,
                      Colour border)
{
  switch(fTarget)
  {
    case GL_TEXTURE_3D:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R,
                      kGLWrapModes[static_cast<uint8_t>(rWrap)]);
    case GL_TEXTURE_2D:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                      kGLWrapModes[static_cast<uint8_t>(tWrap)]);
    case GL_TEXTURE_1D:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                      kGLWrapModes[static_cast<uint8_t>(sWrap)]);
    break;
  }

  /* Check if any of the modes were clamped to the border colour. */
  if(kGLWrapModes[static_cast<uint8_t>(sWrap)] == GL_CLAMP_TO_BORDER ||
     kGLWrapModes[static_cast<uint8_t>(tWrap)] == GL_CLAMP_TO_BORDER ||
     kGLWrapModes[static_cast<uint8_t>(rWrap)] == GL_CLAMP_TO_BORDER)
  {
    /* Set the border colour. */
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border.data());
  }
}

/******************************************************************************/
void Texture::bind(uint32_t texUnit)
{
  /* Check if the correct texture unit is active. Not entirely sure if this
   * really helps performance since the glBindTexture will cause a stage change
   * regardless, but for now let's stick with it. */
  if(fActiveTexUnit != texUnit)
  {
    /* Activate the texture unit. */
    glActiveTexture(GL_TEXTURE0 + texUnit);

    /* Set the active texture unit. */
    fActiveTexUnit = texUnit;
  }

  /* Bind the texture. */
  glBindTexture(fTarget, fID);
}

/******************************************************************************/
void Texture::unbind(uint32_t texUnit)
{
  /* Check if the correct texture unit is active. */
  if(fActiveTexUnit != texUnit)
  {
    /* Activate the texture unit. */
    glActiveTexture(GL_TEXTURE0 + texUnit);

    /* Set the active texture unit. */
    fActiveTexUnit = texUnit;
  }

  /* Bind the NULL object. */
  glBindTexture(fTarget, 0);
}

