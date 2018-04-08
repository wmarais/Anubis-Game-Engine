#ifndef ANUBIS_GRAPHICS_PIXEL_MAP_HPP
#define ANUBIS_GRAPHICS_PIXEL_MAP_HPP

#include "../Common.hpp"

namespace Anubis
{
  namespace Graphics
  {
    /***********************************************************************//**
     * A class for loading, storing and manipulating pixel data. It is the basic
     * object required for creating textures. The origin (0,0) of the pixel map
     * is in the top left corner. This allows for direct translation between
     * the pixel map and OpenGL textures.
     *
     *   0,0       width,0
     *    +-------------+
     *    |             |
     *    |             |
     *    |             |
     *    |             |
     *    |             |
     *    +-------------+
     * 0, height   width,height
     **************************************************************************/
    class PixelMap
    {

      static const size_t kTGAHeaderLen = 6;

      static const size_t kBPP[5];
    public:

      /*********************************************************************//**
       * The supported images types.
       ************************************************************************/
      enum class PixelTypes : uint8_t
      {
        /** A gray scale pixel map that can be used as an alpha map. */
        Gray   = 0,
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

      PixelMap(const uint8_t * data, size_t width, size_t height,
               PixelTypes type);

      /*********************************************************************//**
       * Creates a blank pixel map of the specified width, height and type.
       * @param width
       * @param height
       * @param type
       ************************************************************************/
      PixelMap(size_t width, size_t height, PixelTypes type);

      PixelMap & operator = (PixelMap && mv);

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

      size_t width() const;
      size_t height() const;
      size_t bpp() const;

      void resize(size_t width, size_t height);

      /*********************************************************************//**
       * Create a TGA image file from the bitmap contents.
       *
       * @return  The TGA image's raw binary data that can be directly written
       *          to a .tga file.
       ************************************************************************/
      std::vector<uint8_t> toTGA() const;

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
