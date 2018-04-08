#ifndef ANUBIS_GRAPHICS_TEXT_RENDERER_HPP
#define ANUBIS_GRAPHICS_TEXT_RENDERER_HPP

#include "Colour.hpp"
#include "../Common.hpp"
#include "../Math.hpp"

namespace Anubis
{
  namespace Graphics
  {
    class TextRenderer
    {
      /** A structure that contains the required information for rendering a
       * particular code point. */
      class Glyph;

      /** The texture atlas used by the text renderer to cache the bitmaps of
       * the glyphs. */
      class GlyphAtlas;

      /** The object storing the platform specific information of the class. */
      struct Data;

      /** A reference to the platform specific information. */
      //std::unique_ptr<Data> fData;

    public:

      class Font;

      /*********************************************************************//**
       * Create a text renderer object with the specified cache length. Since
       * the text renderer tries to batch draw calls, the longer the cache, the
       * less draw calls will be invoked. However a cache length that is too
       * long will just waste memory.
       *
       * @param cacheLen  The cache length of polygons corresponding to the
       *                  glyph being rendered.
       ************************************************************************/
      TextRenderer(size_t cacheLen = 256);

      /*********************************************************************//**
       * Add the font at the specified path to the text renderer and return a
       * pointer to the created font. Note the user must not delete manually
       * delete this pointer since it is managed by the TextRenderer object.
       *
       * @param path  The path to the font to be loaded.
       * @param uuid  The UUID to uniquely identify the font by. If none is
       *              provided, the autogenerated one is used.
       * @return      The pointer to the created font. Pass this pointer to the
       *              drawText() function.
       ************************************************************************/
      Font * addFont(const std::string & path,
                     const Common::UUID & uuid = Common::UUID());

      /*********************************************************************//**
       * Draw a UTF8 unicode string in world space.
       *
       * @param text    The UTF8 string to draw.
       * @param font    The font to use for drawing.
       * @param height  The height to render the glyph.
       * @param colour  The colour of the string.
       * @param trans   The world transformation of the text.
       ************************************************************************/
      void drawText(const std::string & text, const Font * font, float height,
                    const Colour & colour, const Math::Matrix4f & trans);
    };
  }
}


#endif /* ANUBIS_GRAPHICS_TEXT_RENDERER_HPP */
