#ifndef ANUBIS_GRAPHICS_PIXEL_MAP_HPP
#define ANUBIS_GRAPHICS_PIXEL_MAP_HPP

#include "../Common/Memory.hpp"

namespace Anubis
{
  namespace Graphics
  {
    /***********************************************************************//**
     * A class for loading, storing and manipulating pixel data. It is the basic
     * object required for creating textures.
     **************************************************************************/
    class PixelMap
    {
    public:

      /*********************************************************************//**
       * The supported images types.
       ************************************************************************/
      enum class PixelTypes : uint8_t
      {
        RGB,
        RGBA,
        BGR,
        BGRA
      };

      /*********************************************************************//**
       * Load the pixel map data from a raw TGA data block.
       *
       * @param data
       ************************************************************************/
      PixelMap(const std::vector<uint8_t> data);

      /*********************************************************************//**
       * The type of the image, either RGB, RGBA, BGR, BGRA.
       *
       * @return The pixel type of the image.
       ************************************************************************/
      PixelTypes pixelType() const;

      /*********************************************************************//**
       * Conover the image type to the specified type and returning a refence
       * to this object once done.
       *
       * @param type
       * @return
       ************************************************************************/
      PixelMap & changePixelType(PixelTypes & pixelType);

    private:
      /** The bit pattern for a compressed TGA. */
      static const uint8_t kCompHeader[12];

      /** Thje bit patter for an uncompressed TGA. */
      static const uint8_t kUncompHeader[12];

      /** The pixel map's pixel type. */
      PixelTypes fType;

      /** The width of the pixel map in pixels. */
      size_t fWidth;

      /** The height of the pixel map in pixels. */
      size_t fHeight;

      /** The number of bytes per pixel. */
      size_t fBPP;

      /** The data of pixel map. */
      std::vector<uint8_t> fData;

      /** Load a TGA image. */
      void loadTGA(const std::vector<uint8_t> & data, bool compressed);
    };
  }
}

#endif /* ANUBIS_GRAPHICS_PIXEL_MAP_HPP */
