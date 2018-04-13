#ifndef ANUBIS_GRAPHICS_TEXTURE_HPP
#define ANUBIS_GRAPHICS_TEXTURE_HPP

#include "../Common.hpp"
#include "PixelMap.hpp"
#include "Colour.hpp"

namespace Anubis
{
  namespace Graphics
  {
    class Texture
    {
    protected:

      /** The number of supported texture formats. */
      static const size_t kFormatsCount = 5;

      /** The number of supported wrap modes. */
      static const size_t kWrapModesCount = 4;

      /** The list of OpenGL formats to translated the PixelMap::PixelTypes. */
      static const int32_t kFormats[kFormatsCount];

      /** The list of OpenGL wrap modes to translate the WrapModes. */
      static const int32_t kGLWrapModes[kWrapModesCount];

      /** The currently active texture unit. */
      static uint32_t fActiveTexUnit;

      /** The texture object ID. */
      uint32_t fID;

      /** The OpenGL texture target. */
      uint32_t fTarget;

      /** Indicate if the texture has mip maps. */
      bool fHasMipMaps;

      /** This object can't be simply copied and it usually an error for users
       * to try, thus the copy constructor is deleted. */
      Texture(const Texture &) = delete;

      /** This object can't be simply copied and it usually an error for users
       * to try, thus the assignment operator is deleted. */
      Texture & operator = (const Texture &) = delete;

      /*********************************************************************//**
       * Create a texture object for the specific OpenGL target with mip maps
       * enabled or disabled. The constructor is declare as protected since it
       * should never be accessible to the public. (I.e. users should never use
       * it to directly to create textures.)
       *
       * @param target      The OpenGL target to create the texture for. I.e.
       *                    GL_TEXTURE_1D, GL_TEXTURE_2D, etc.
       * @param hasMipMaps  Indicate it mip maps should be generated or not.
       ************************************************************************/
      Texture(uint32_t target, bool hasMipMaps);

    public:

      /** The image texture filtering options that are available for pixel
       * interpolation. */
      enum class Filters : uint8_t
      {
        /** Disable texture filtering / interpolation. This will produce the
         * lowest rendering quality, but the highest performance. */
        Nearest     = 0,

        /** Perform bilinear interpolation. */
        Bilinear    = 1,

        /** Perform trilinear interpolation. This will only have a visible
         * effect on textures with mip maps. */
        Trilinear   = 2,

        /** Perform anisotropic filtering. This will only have a visible
         * effect on textures with mip maps. This will produce the highest
         * rendering quality, but is also has the highest performance
         * penalty. */
        Anisotropic = 3
      };

      /** The list of supported wrap modes to describe how pixel lookups must
       * be performed for pixels outside the normalised texture coordinate
       * range of 0.0f - 1.0f. */
      enum class WrapModes : uint8_t
      {
        /** Repeat the texture infinitely. */
        Repeat          = 0,

        /** Repeat and mirror infinitely. */
        Mirror          = 1,

        /** Clamp the pixel colours to the colours on the edges of the
         * texture. */
        ClampToEdge     = 2,

        /** Set the pixels outside the range to a user defined colour. */
        ClampToBorder   = 3
      };

      /*********************************************************************//**
       * Deallocate the texture on the video card (and also declare the
       * destructor as virtual to allow inheriting class' destructor to be
       * invoked upon destruction).
       ************************************************************************/
      virtual ~Texture();

      /*********************************************************************//**
       * Set the texture filtering to use for minifying (minFilter) and
       * magnifying (magFilter) operations. The texture must be bound before
       * setting the filter (call bind()).
       *
       * @param filter    The filter method to use.
       * @param aniLevel  The anisotropy level to use. This must always a
       *                  that is power of two, i.e. aniLevel = 2^N.
       ************************************************************************/
      virtual void setFilter(Filters filter, uint32_t aniLevel = 2);

      /*********************************************************************//**
       * Set the wrapping modes for the textures.
       *
       * @param sWrap   How the S coordinates shall be wrapped.
       * @param tWrap   How the T coordinates shall be wrapped.
       * @param rWrap   How the R coordinates shall be wrapped.
       * @param border  The colour to use for ClampToBorder.
       ************************************************************************/
      virtual void setWrap(WrapModes sWrap, WrapModes tWrap = WrapModes::Repeat,
                           WrapModes rWrap = WrapModes::Repeat,
                           Colour border = kBlack);

      /*********************************************************************//**
       * Bind the texture to the specified texture unit. If only a single
       * texture is used then no parameter needs to be specified. If
       * multitexturing is used then the texture unit should also be specified.
       *
       * @param texUnit The texture unit to bind the texture too. The value is
       *                in the range 0 >= texUnit <= 31 for a maximum on 32
       *                texture units. Though this number is supported by the
       *                OpenGL API, the targeted hardware may not support up to
       *                32 texture units.
       ************************************************************************/
      virtual void bind(uint32_t texUnit = 0);

      /*********************************************************************//**
       * Unbind the use of the texture on the specific texture unit. This
       * will also deactivate the associated texture unit. It is not necessary
       * or recommended to call unbind between renders that use the same number
       * of texture units since this will cause unecisary state changes.
       *
       * @param texUnit The texture unit to unbind from.
       ************************************************************************/
      virtual void unbind(uint32_t texUnit = 0);
    };
  }
}

#endif /* ANUBIS_GRAPHICS_TEXTURE_HPP */
