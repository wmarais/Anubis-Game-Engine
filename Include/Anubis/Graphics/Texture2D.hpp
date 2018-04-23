#ifndef ANUBIS_GRAPHICS_TEXTURE_2D_HPP
#define ANUBIS_GRAPHICS_TEXTURE_2D_HPP

#include "Texture.hpp"
#include "PixelMap.hpp"

namespace Anubis
{
  namespace Graphics
  {
    class Texture2D final : public Texture
    {
      /** The width of the texture. */
      uint32_t fWidth;

      /** The height of the texture. */
      uint32_t fHeight;

      /** The pixel format of the texture. */
      PixelMap::PixelTypes fPixelType;

      Texture2D(const Texture2D &) = delete;

      Texture & operator = (const Texture2D &) = delete;

    public:

      Texture2D(const PixelMap & pixelMap, bool genMipMaps = true);
      ~Texture2D();

      /*********************************************************************//**
       * Update the content of the entire texture. A diffirent sized pixel map
       * or even pixel type can be supplied, however this will result in a
       * reallocation of the texture structure on the video card and is quite
       * slow. (So try and keep the texture size and pixel format the same
       * since this will result in much better performance overall.)
       *
       * @param pixelMap  The pixelmap to use for updating the texture data.
       ************************************************************************/
      void setData(const PixelMap & pixelMap);
    };
  }
}

#endif /* ANUBIS_GRAPHICS_TEXTURE_2D_HPP */
