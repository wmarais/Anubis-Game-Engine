#include "../../../Include/Anubis/Graphics/TextRenderer.hpp"
#include "../../../Include/Anubis/Graphics/PixelMap.hpp"
#include "../../../Include/Anubis/Common/System.hpp"
#include "../../../Include/Anubis/Math/Vector4f.hpp"
#include "../../../Include/Anubis/Physics/BoundingRect.hpp"

/** Include the FreeType2 headers for rendering text. */
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace Anubis::Math;
using namespace Anubis::Physics;
using namespace Anubis::Graphics;


const std::string TextRenderer::kVertexShaderCode =
"#version 320 core \n"
"layout (location = 0) in vec4 vertex; \n"
"layout (location = 1) in vec2 texCoord; \n"
"out vec2 texCoordV; \n"
"uniform mat4 transform; \n"
"void main() { \n"
"  gl_Position = transform * vertex; \n"
"  texCoordV = texCoord; \n"
"}\n";

const std::string TextRenderer::kFragmentShaderCode =
"#version 320\n"
"in vec2 texCoordV; \n"
"out vec4 colorOut; \n"
"uniform sampler2D texture; "
"void main() {\n"
"  colorOut = vec4(texCoord, 0.0, 0.0); \n"
"}\n";


/*##############################################################################
##############################################################################*/
struct TextRenderer::Glyph final
{
  /** The code point of the glyph. */
  char32_t kCodePoint;

  /** The height of the font used to render this glyph. */
  const size_t kFontHeight;

  /** The font that was used to render the glyph. */
  const TextRenderer::Font * kFont;

  /** The area of the glyph. */
  const float kArea;

  /** The width of the glyph. */
  const float kWidth;

  /** The height of the glyph. */
  const float kHeight;

  /** The x bearing of the glyph. */
  const float kBearingX;

  /** They y bearing of the glyph. */
  const float kBearingY;

  /** How far to advance before rendering the next glyph. */
  const float kAdvanceX;

  const float kAdvanceY;

  /** The X position of the glyph in the glyph atlas. */
  float fX;

  /** The Y position of the flyph in the glyph atlas. */
  float fY;

  /** The pixel map for the glyph. */
  std::unique_ptr<PixelMap> fPixelMap;

  /** Delete the copy constructor to prevent expensive copies. */
  Glyph(const Glyph&) = delete;

  /** Delete the asignment operator to prevent expensive copies. */
  Glyph & operator = (const Glyph&) = delete;

  /*************************************************************************//**
   * Create a new glyph using the font face information from free type.
   *
   * @param face  The freetype font face containing the current glyph
   *              information.
   ****************************************************************************/
  Glyph(uint32_t codePoint, const FT_Face & face, size_t fontHeight);
};

/******************************************************************************/
TextRenderer::Glyph::Glyph(uint32_t codePoint, const FT_Face & face,
  size_t fontHeight) :
  kCodePoint(codePoint),
  kFontHeight(fontHeight),
  kArea(face->glyph->bitmap.width * face->glyph->bitmap.rows),
  kWidth(face->glyph->bitmap.width),
  kHeight(face->glyph->bitmap.rows),
  kBearingX(face->glyph->bitmap_left),
  kBearingY(face->glyph->bitmap_top),
  kAdvanceX((face->glyph->advance.x) >> 6),
  kAdvanceY((face->glyph->advance.y) >> 6),
  fX(0.0f),
  fY(0.0f)
{
  /* Create and assign the pixel map. */
  fPixelMap = std::make_unique<PixelMap>(face->glyph->bitmap.buffer,
                                         face->glyph->bitmap.width,
                                         face->glyph->bitmap.rows,
                                         PixelMap::PixelTypes::Gray);
}

/*##############################################################################
##############################################################################*/
class TextRenderer::Font final
{
  /** The path of the font. */
  std::string fPath;

  /** The uniqueue identifier of the font. */
  Anubis::Common::UUID fUUID;

  /** The font face used for rendering glyphs. */
  FT_Face fFTFace;

  /** Delete the copy constructor. */
  Font(const Font&) = delete;

  /** Delete the assignment operator. */
  Font operator = (const Font &) = delete;

public:

  /*************************************************************************//**
   * Create a new font object.
   *
   * @param lib   The FreeType library instance to use.
   * @param path  The path to the font to load.
   * @param uuid  The uuid that will be used to identify this font.
   ****************************************************************************/
  Font(FT_Library &lib, const std::string & path,
       const Anubis::Common::UUID & uuid);

  /*************************************************************************//**
   * Release the font face object associated with this font.
   ****************************************************************************/
  ~Font();

  /*************************************************************************//**
   * Render the glyph at the selected code point to the supplied pixel map at
   * the specified text height.
   *
   * @param codePoint The code point to render.
   * @param pixelMap  The pixel map to render too.
   * @param height    The height of the text to render.
   ****************************************************************************/
  std::shared_ptr<Glyph> render(char32_t codePoint, size_t fontHeight);
};

/******************************************************************************/
TextRenderer::Font::Font(FT_Library & lib, const std::string & path,
  const Anubis::Common::UUID & uuid) : fPath(path), fUUID(uuid)
{
  /* Try to load the specified font. */
  if(FT_New_Face(lib, fPath.c_str(), 0, &fFTFace) != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Failed to load font: " << path << ".");
  }
}

/******************************************************************************/
TextRenderer::Font::~Font()
{
  /* Close the font. */
  FT_Done_Face(fFTFace);
}

/******************************************************************************/
std::shared_ptr<TextRenderer::Glyph> TextRenderer::Font::render(
    char32_t codePoint, size_t fontHeight)
{
  /* Set the height of the font and let TrueType calculate the appropriate width
   * of the glyphs (hence width = 0 in this call). */
  if(FT_Set_Pixel_Sizes(fFTFace, 0, fontHeight) != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Failed to set font height.");
  }

  /* Render the glyph. */
  if(FT_Load_Char(fFTFace, codePoint, FT_LOAD_RENDER) != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Failed to load glyph for code point: "
                                   << codePoint << ".");
  }

  /* Create and return the glyph. */
  return std::make_shared<Glyph>(codePoint, fFTFace, fontHeight);
}

/*##############################################################################
##############################################################################*/
class TextRenderer::GlyphAtlas final
{
  /** The default starting dimensions of the pixel map. */
  static const size_t kDefaultDim = 512;

  /** The maximum with and height of the atlas pixel map. This is tuned to work
   * with most video cards. */
  static const size_t kMaxDim = 8192;

  /** The node that forms the binary tree used for packing the glyph. */
  struct Node
  {
    /** The left child of this node. */
    std::unique_ptr<Node> fLeft;

    /** The right child of this node. */
    std::unique_ptr<Node> fRight;

    /** The bonding rectangle of this node. */
    BoundingRect fRect;

    /** The glyph inserted at this node. */
    std::shared_ptr<Glyph> fGlyph;

    /***********************************************************************//**
     * Check if the current node is a leaf node or not.
     *
     * @return  True if the node is a leaf node, else  false.
     **************************************************************************/
    bool isLeaf() const;

    /***********************************************************************//**
     * Insert a new glyph into the atlas and pack it properly.
     *
     * @param glyph   The glyph to be inserted intgo the atlas.
     * @return        True if the glyph was packed or false if the glyph can not
     *                be packed because (1) this is a leaf node and it allready
     *                contains a glyph, (2) there is not enough space to fit
     *                the glyph.
     **************************************************************************/
    bool insert(std::shared_ptr<Glyph> glyph, PixelMap &dst);
  };

  /** The root node of the binary tree used to packe the glyphs. */
  std::unique_ptr<Node> fRoot;

  /** The comparator object used to sort the multiset of glyphs. */
  struct Compare
  {
    /** Sort the glyphs by code point. */
    bool operator () (const std::shared_ptr<Glyph> & lhs,
                      const std::shared_ptr<Glyph> & rhs)
    { return lhs->kCodePoint < rhs->kCodePoint; }
  };

  /** The instance of the FT library. */
  FT_Library fFTLibrary;

  /** A multiset used to store the glyph. It uses the custom comparator to
   * ensure that the set is sorted by the code point of each glyph. A multiset
   * is used because duplicate code points will exist. */
  std::multiset<std::shared_ptr<Glyph>, Compare> fGlyphs;

  /** The glyphs sorted by their physical area. This is used for packing the
   * glyphs into the glyph atlas. */
  std::vector<std::shared_ptr<Glyph>> fGlyphsByArea;

  /** Indicate if the atlas changed or not. */
  bool fHasChanged;

  /** The list of fonts. */
  std::vector<std::unique_ptr<Font>> fFonts;

  /** Delete the copy constructor. */
  GlyphAtlas(const GlyphAtlas&) = delete;

  /** Delete the assignment operator. */
  GlyphAtlas & operator = (const GlyphAtlas&) = delete;

  /*************************************************************************//**
   * Check if a glyph of the specified code point and font height is allready
   * in the cache.
   *
   * @param codePoint   The code point of the glyph.
   * @param fontHeight  The font height used to rendere the glyph.
   * @return            True if the atlas allready contains the glyph, else
   *                    false if it must be created.
   ****************************************************************************/
  bool contains(char32_t codePoint, size_t fontHeight);

public:

  /** The pixel map of the atlas. */
  std::unique_ptr<PixelMap> fPixelMap;

  /*************************************************************************//**
   * Default constructor to initialise FreeType and created the default glyph
   * atlas.
   ****************************************************************************/
  GlyphAtlas();

  /*************************************************************************//**
   * Destructor to terminal FreeType.
   ****************************************************************************/
  ~GlyphAtlas();

  /*************************************************************************//**
   * Prepare the atlas by adding any non existing code points to the codepoint
   * mapping. This must be called before "pack()".
   *
   * @param font        The font to use for rendering the glyph(s).
   * @param str         The string that need to be rendered.
   * @param fontHeight  The height to render the font at.
   ****************************************************************************/
  bool prepare(Font * font, const std::u32string &str, size_t fontHeight);

  /*************************************************************************//**
   * Tightly pack the glyphs into the glyph (texture) atlas.
   ****************************************************************************/
  void pack();

  /* Get a reference to the glyph. */
  Glyph * find(char32_t codePoint, size_t fontHeight,
                              const Font * font);

  Font * addFont(const std::string & path, const Common::UUID & uuid);
};

/******************************************************************************/
bool TextRenderer::GlyphAtlas::Node::isLeaf() const
{
  return fLeft == nullptr && fRight == nullptr;
}

/******************************************************************************/
bool TextRenderer::GlyphAtlas::Node::insert(std::shared_ptr<Glyph> glyph,
                                            PixelMap & dst)
{
  /* Check if this not a leaf node. If it's not then try to insert the glyph
   * into one of the child nodes. */
  if(!isLeaf())
  {
    /* Try to insert into the left node. */
    bool inserted = fLeft->insert(glyph, dst);

    /* Check if it was not inserted on the left. */
    if(!inserted)
    {
      /* Try to insert to the right. */
      inserted = fRight->insert(glyph, dst);
    }

    /* Return the insert status for this node. */
    return inserted;
  }
  /* Else it is a leaf node. */

  /* Check if there is allready a glyph stored in this node. */
  if(fGlyph != nullptr)
  {
    /* Return false to indicate that this leaf is allready populated. */
    return false;
  }

  /* Check if the glyph will fit into the current rect. */
  if(!fRect.canFit(BoundingRect(0, 0, glyph->kWidth, glyph->kHeight)))
  {
    /* Return false to indicate that leaf node does not fit. */
    return false;
  }

  /* Check if the new glyph fits perfectly. */
  if(fRect.width() == glyph->kWidth && fRect.height() == glyph->kHeight)
  {
    /* Fit the glyph to the node. */
    fGlyph = glyph;

    /* Set the x and y position of the glyph in the atlas. */
    fGlyph->fX = fRect.x();
    fGlyph->fY = fRect.y();

    /* Copy the pixel map into the new image. */
    dst.copyIn(*(fGlyph->fPixelMap.get()), fGlyph->fX, fGlyph->fY);

    /* Return true to indicate it was inserted. */
    return true;
  }

  /* Else the nodes must be split. */

  /* Create thje new children for the node. */
  fLeft = std::make_unique<Node>();
  fRight = std::make_unique<Node>();

  /* Decide which way to split the bounding rect. */
  size_t dw = fRect.width() - glyph->kWidth;
  size_t dh = fRect.height() - glyph->kHeight;

  /* Check if the bounding area must be split vertically. */
  if(dw > dh)
  {
    /* The left node refer to then left bounding rect. */
    fLeft->fRect = BoundingRect(fRect.x(), fRect.y(),
                                glyph->kWidth, fRect.height());

    /* The right node refer to the right bounding rect. */
    fRight->fRect = BoundingRect(fRect.x() + glyph->kWidth, fRect.y(),
                          fRect.width() - glyph->kWidth, fRect.height());
  }
  /* else split the bounding rect horizontally. */
  else
  {
    /* The left node refers to the top bounding rect. */
    fLeft->fRect = BoundingRect(fRect.x(), fRect.y(),
                          fRect.width(), glyph->kHeight);

    /* The right node refers to the bottom bounding rect. */
    fRight->fRect = BoundingRect(fRect.x(), fRect.y() + glyph->kHeight,
                          fRect.width(), fRect.height() - glyph->kHeight);
  }

  /* Insert into the left node. */
  return fLeft->insert(glyph, dst);
}

/******************************************************************************/
TextRenderer::GlyphAtlas::GlyphAtlas()
{
  /* Initialise the free type library. */
  if(FT_Init_FreeType(&fFTLibrary) != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Failed to initialise FreeType.");
  }

  /* Do this to resolve issue with gcc. */
  size_t dim = kDefaultDim;

  /* Create the default pixel map using the default atlas size. */
  fPixelMap = std::make_unique<PixelMap>(dim , dim,
                                         PixelMap::PixelTypes::Gray);
}

/******************************************************************************/
TextRenderer::GlyphAtlas::~GlyphAtlas()
{
  /* Clear all the fonts. */
  fFonts.clear();

  /* Close the free type library. */
  FT_Done_FreeType(fFTLibrary);
}

/******************************************************************************/
bool TextRenderer::GlyphAtlas::contains(char32_t codePoint, size_t fontHeight)
{
  /* Find the first occurence of the code point in the atlas. */
  auto start = std::lower_bound(fGlyphs.begin(), fGlyphs.end(), codePoint,
    [](const std::shared_ptr<Glyph> & lhs, const char32_t & rhs)
      -> bool { return lhs->kCodePoint < rhs; });

  /* Check if it was found. */
  if(start != fGlyphs.end())
  {
    /* Find the last occurance of the glyph. */
    auto end = std::upper_bound(start, fGlyphs.end(), codePoint,
      [](const char32_t & lhs, const std::shared_ptr<Glyph> & rhs)
        -> bool { return lhs < rhs->kCodePoint; });

    /* Search for the first glyph with the correct font height in the range,
     * there should never be more than one. */
    auto exact = std::find_if(start, end,
      [&fontHeight](const std::shared_ptr<Glyph> & glyph)
        -> bool { return glyph->kFontHeight == fontHeight; });

    /* Check if the exact codepoint and font height has been found. */
    if(exact != fGlyphs.end())
    {
      /* Return true to indicate that the exact match was found. */
      return true;
    }
  }

  /* Otherwise the exact match was not found. */
  return false;
}

/******************************************************************************/
TextRenderer::Glyph * TextRenderer::GlyphAtlas::find(
    char32_t codePoint, size_t fontHeight, const Font * font)
{
  /* Find the first occurence of the code point in the atlas. */
  auto start = std::lower_bound(fGlyphs.begin(), fGlyphs.end(), codePoint,
    [](const std::shared_ptr<Glyph> & lhs, const char32_t & rhs)
      -> bool { return lhs->kCodePoint < rhs; });

  /* Check if it was found. */
  if(start != fGlyphs.end())
  {
    /* Find the last occurance of the glyph. */
    auto end = std::upper_bound(start, fGlyphs.end(), codePoint,
      [](const char32_t & lhs, const std::shared_ptr<Glyph> & rhs)
        -> bool { return lhs < rhs->kCodePoint; });

    /* Search for the first glyph with the correct font height in the range,
     * there should never be more than one. */
    auto exact = std::find_if(start, end,
      [&fontHeight, &font](const std::shared_ptr<Glyph> & glyph)
        -> bool { return glyph->kFontHeight == fontHeight &&
                         glyph->kFont == font; });

    /* Check if the exact codepoint and font height has been found. */
    if(exact != fGlyphs.end())
    {
      /* Return the glyph that was found. */
      return exact->get();
    }
  }

  /* Throw an exception if the glyph was not found because it should be in
   * here unless something is broken. */
  ANUBIS_THROW_RUNTIME_EXCEPTION("Failed to find glyph: " << codePoint);
}

/******************************************************************************/
bool TextRenderer::GlyphAtlas::prepare(Font * font, const std::u32string & str,
                                       size_t fontHeight)
{
  /* Iterate through the whole string. */
  for(auto codePoint : str)
  {
    /* Check if the glyph is missing from the atlas. */
    if(!contains(codePoint, fontHeight))
    {
      /* Create the glyph. */
      std::shared_ptr<Glyph> glyph = font->render(codePoint, fontHeight);

      /* Insert the glyph into the set of glyphs sorted by code point. */
      fGlyphs.insert(glyph);

      /* Set the glyph into the vector of glyphs sorted by height. */
      fGlyphsByArea.push_back(glyph);

      /* Mark the atlas as having changed. */
      fHasChanged = true;
    }
  }

  /* Indicate if the atlas changed or not. */
  return fHasChanged;
}

/******************************************************************************/
void TextRenderer::GlyphAtlas::pack()
{
  /* Check if the atlas has not changed. */
  if(!fHasChanged)
  {
    /* No need to rebuild it then. */
    return;
  }

  /* Clear the has changed flag. */
  fHasChanged = false;

  /* Sort the vector in descending order of glyph Area. */
  std::sort(fGlyphsByArea.begin(), fGlyphsByArea.end(),
    [] (const std::shared_ptr<Glyph> & lhs, const std::shared_ptr<Glyph> & rhs)
      -> bool { return lhs->kArea > rhs->kArea; });

  /* Set dimension to scale too. (Since the width and the height is the same
   * we only need to track one or the other.) */
  size_t resDim = fPixelMap->width();

  /* The current glyph iterator. */
  auto curIter = fGlyphsByArea.begin();

  do
  {
    /* Make sure the pixel map size is safe. */
    if(resDim > kMaxDim)
    {
      ANUBIS_THROW_RUNTIME_EXCEPTION("The requested pixe-l map size is to "
        "large: " << resDim << "x" << resDim << ".");
    }

    /* Reset the iterator. */
    curIter = fGlyphsByArea.begin();

    /* Set the size of the pixelmap. */
    fPixelMap->resize(resDim, resDim);

    /* Create a new root node. */
    fRoot = std::make_unique<Node>();

    /* Set the new bounding area to match the pixel map. */
    fRoot->fRect = BoundingRect(0, 0, fPixelMap->width(), fPixelMap->height());

    /* Iterate through all the glyphs. */
    for(; curIter != fGlyphsByArea.end(); curIter++)
    {
      /* Check if the glyph was sucessfully packed. */
      if(fRoot->insert(*curIter, *(fPixelMap.get())) == false)
      {
        /* Break out of the loop since the pixelmap must be resized. */
        break;
      }
    }

    /* Increment the size of the pixel map. Since all the textures are to be
     * powers of two, simply multiply by 2. */
    resDim *= 2;
  }
  while(curIter != fGlyphsByArea.end());
}

/******************************************************************************/
TextRenderer::Font * TextRenderer::GlyphAtlas::addFont(const std::string & path,
                             const Common::UUID & uuid)
{
  /* Create the new font. */
  std::unique_ptr<Font> font = std::make_unique<Font>(fFTLibrary, path, uuid);

  /* Get a reference to the pointer. */
  Font * ptr = font.get();

  /* Add the font to the list of fonts. */
  fFonts.push_back(std::move(font));

  /* Return the pointer to the font. */
  return ptr;
}

///******************************************************************************/
//struct TextRenderer::Data
//{
//  /** The number of vertices required to render a glyph. Since quads are used,
//   * four vertices is required. */
//  static const uint16_t kVertsPerGlyph = 4;

//  /** The number of indices that is required to render a single glyph. Since two
//   * triangles are used to render a single quad, six indices are required. */
//  static const uint16_t kIndicesPerGlyph = 6;

//  /** The maximum number of glyphs to cache. */
//  const size_t kMaxGlyphCache;

//  /** The maximum number of vertices to cache. */
//  const size_t kMaxCachedVerts;

//  /** The ID of the vertex buffer. */
//  GLuint fVertBuffID;

//  /** The ID of the index buffer. */
//  GLuint fIndexBuffID;

//  GLuint fVAOID;

//  /** The vertex cache. */
//  std::vector<Vector4f> fVertices;


//  Data(const std::string &fontFile, const size_t fontHeight, size_t cacheLen);
//  ~Data();


//  void initCache();
//};

///******************************************************************************/
//TextRenderer::Data::Data(const std::string & fontFile, const size_t fontHeight,
//                         size_t cacheLen) :
//  kMaxGlyphCache(cacheLen),
//  kMaxCachedVerts(cacheLen *  kVertsPerGlyph)
//{


//  /* Initialise the render cache. */
//  initCache();
//}

///******************************************************************************/
//TextRenderer::Data::~Data()
//{

//}

/******************************************************************************/
void TextRenderer::initCache()
{
  /* Calculate how many vertices are required. Since rectangles are required
   * that is 4 vertices per rectangle. */
  size_t reqVerts = kGlyphCacheLen * kVertsPerGlyph;

  /* Calculate how many indices must be created. Since the rectangles are built
   * from two triangles, that is 6 indexes per faces. */
  size_t reqIndices = kGlyphCacheLen * kIndicesPerGlyph;

  /* The vector that stores the list of indexes. This will never change however
   * we may only choose to draw a subject of the list (since the cache may not
   * always be full). */
  std::vector<uint16_t> indices;
  indices.resize(reqIndices);

  /* Calculate all the indices for each face. */
  for(uint16_t i = 0; i < reqIndices; i+= kIndicesPerGlyph)
  {
    /* The indices of the first face. */
    indices.push_back(i);
    indices.push_back(i + 1);
    indices.push_back(i + 2);

    /* The indices of the second face. */
    indices.push_back(i + 1);
    indices.push_back(i + 3);
    indices.push_back(i + 2);
  }

  /* Create the indices buffer and load the index data. */
  glGenBuffers(1, &fIndexBuffID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fIndexBuffID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t),
               indices.data(), GL_STATIC_DRAW);

  /* Reserver enough space in the vertices. */
  fVertsAndTexCoords.resize(reqVerts * kFloatsPerVert);

  /* Create the vertex buffer. */
  glGenBuffers(1, &fVertBuffID);
  glBindBuffer(GL_ARRAY_BUFFER, fVertBuffID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * fVertsAndTexCoords.size(),
               fVertsAndTexCoords.data(), GL_STREAM_DRAW);
}

/******************************************************************************/
TextRenderer::TextRenderer(size_t cacheLen) : kGlyphCacheLen(cacheLen)
{
  /* Create the glyph atlas. */
  fGlyphAtlas = std::make_unique<GlyphAtlas>();

  /* Creat the texture to render the glyph. */
  fTexture = std::make_unique<Texture2D>(*(fGlyphAtlas->fPixelMap.get()));

//  /* The list of shaders. */
//  std::vector<std::shared_ptr<Shader>> shaders =
//  {
//    std::make_shared<Shader>(Shader::Types::Vertex, kVertexShaderCode),
//    std::make_shared<Shader>(Shader::Types::Fragment, kFragmentShaderCode)
//  };

//  /* Create the shader program. */
//  fShaderProgram = std::make_unique<ShaderProgram>(shaders);
}

/******************************************************************************/
TextRenderer::~TextRenderer()
{

}

/******************************************************************************/
TextRenderer::Font * TextRenderer::addFont(const std::string & path,
               const Common::UUID & uuid)
{
  return fGlyphAtlas->addFont(path, uuid);
}

///******************************************************************************/
//void TextRenderer::drawText(const std::string & text,
//                            const Math::Vector4f & pos, const Colour & colour)
//{
//  /* Clear the current cache. */
//  fData->fVertices.clear();

//  /* Iterate through all the characters in the string. */
//  for(size_t i = 0; i < text.length(); i++)
//  {
//    /* Calculate the vertices of the string. */



//    /* Check if cache is full or the end of the string is reached. */
//    if(fData->fVertices.size() > 0 &&
//       (fData->fVertices.size() >= fData->kMaxCachedVerts ||
//        i >= text.length()))
//    {
//      /* Bind the vertex buffer and update it's contents. */
//      glBindBuffer(GL_ARRAY_BUFFER, fData->fVertBuffID);
//      glBufferSubData(GL_ARRAY_BUFFER, 0,
//                      sizeof(Vector4f) * fData->fVertices.size(),
//                      fData->fVertices.data());

//      /* Bind the indices array. Nothing to update here. */
//      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fData->fIndexBuffID);

//      /* Render the current cache contents. */
//      glDrawElements(GL_TRIANGLES,
//                     (fData->fVertices.size() / Data::kVertsPerGlyph) *
//                     Data::kIndicesPerGlyph, GL_UNSIGNED_SHORT, nullptr);
//    }
//  }
//}

/******************************************************************************/
void TextRenderer::drawTextSimple(const std::string & text, Font * font,
  size_t height, const Colour & colour, const Math::Matrix4f & trans)
{
  /* The UTF-8 - UTF-32 standard conversion facet. */
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;

  /* Conver the text into UTF32. */
  std::u32string u32Text = cvt.from_bytes(text);

  /* Prepare the glyph atlas to render the string. */
  if(fGlyphAtlas->prepare(font, u32Text, height))
  {
    /* Pack the atlas. */
    fGlyphAtlas->pack();

    /* Update the texture with the new atlas pixel map. */
    fTexture->setData(*(fGlyphAtlas->fPixelMap.get()));
  }

  /* The number of faces in the queue. */
  size_t glyphIndex = 0;

  /* The x offset from the origin position. */
  Vector4f advancePos;

  /* Iterate through all the characters in the string. */
  for(auto codePoint : u32Text)
  {
    /* Get a reference to the glyph. */
    Glyph * glyph = fGlyphAtlas->find(codePoint, height, font);

    /* The current insert position. */
    size_t vecPos = glyphIndex * kFloatsPerVert * kVertsPerGlyph;

    /* Create the points for the face. */
    Vector4f v0 = trans * (advancePos +
        Vector4f::makePosition(0, 0, 0));

    Vector4f v1 = trans * (advancePos +
        Vector4f::makePosition(0, glyph->kHeight, 0));

    Vector4f v2 = trans * (advancePos +
        Vector4f::makePosition(glyph->kWidth, 0, 0));

    Vector4f v3 = trans * (advancePos +
        Vector4f::makePosition(glyph->kWidth, glyph->kHeight, 0));

    /* Calculate the texture coordinates. */
    Vector2f t0(static_cast<float>(glyph->fX) /
                static_cast<float>(fGlyphAtlas->fPixelMap->width()),
                static_cast<float>(glyph->fY) /
                static_cast<float>(fGlyphAtlas->fPixelMap->height()));

    Vector2f t1(static_cast<float>(glyph->fX) /
                static_cast<float>(fGlyphAtlas->fPixelMap->width()),
                static_cast<float>(glyph->fY + glyph->kHeight) /
                static_cast<float>(fGlyphAtlas->fPixelMap->height()));

    Vector2f t2(static_cast<float>(glyph->fX + glyph->kWidth) /
                static_cast<float>(fGlyphAtlas->fPixelMap->width()),
                static_cast<float>(glyph->fY) /
                static_cast<float>(fGlyphAtlas->fPixelMap->height()));

    Vector2f t3(static_cast<float>(glyph->fX + glyph->kWidth) /
                static_cast<float>(fGlyphAtlas->fPixelMap->width()),
                static_cast<float>(glyph->fY + glyph->kHeight) /
                static_cast<float>(fGlyphAtlas->fPixelMap->height()));

    /* Calculate the advance position for the next glyph. */
    advancePos += Vector4f(glyph->kAdvanceX, glyph->kAdvanceY);

    /* Insert the vertex in to the array. */
    fVertsAndTexCoords[vecPos++] = v0.x();
    fVertsAndTexCoords[vecPos++] = v0.y();
    fVertsAndTexCoords[vecPos++] = v0.z();
    fVertsAndTexCoords[vecPos++] = v0.w();

    fVertsAndTexCoords[vecPos++] = t0.x();
    fVertsAndTexCoords[vecPos++] = t0.y();

    fVertsAndTexCoords[vecPos++] = v1.x();
    fVertsAndTexCoords[vecPos++] = v1.y();
    fVertsAndTexCoords[vecPos++] = v1.z();
    fVertsAndTexCoords[vecPos++] = v1.w();

    fVertsAndTexCoords[vecPos++] = t1.x();
    fVertsAndTexCoords[vecPos++] = t1.y();


    fVertsAndTexCoords[vecPos++] = v2.x();
    fVertsAndTexCoords[vecPos++] = v2.y();
    fVertsAndTexCoords[vecPos++] = v2.z();
    fVertsAndTexCoords[vecPos++] = v2.w();

    fVertsAndTexCoords[vecPos++] = t2.x();
    fVertsAndTexCoords[vecPos++] = t2.y();


    fVertsAndTexCoords[vecPos++] = v3.x();
    fVertsAndTexCoords[vecPos++] = v3.y();
    fVertsAndTexCoords[vecPos++] = v3.z();
    fVertsAndTexCoords[vecPos++] = v3.w();

    fVertsAndTexCoords[vecPos++] = t3.x();
    fVertsAndTexCoords[vecPos++] = t3.y();

    /* Increment the glyph index. */
    glyphIndex++;


  }




  PixelMap tempPixMap(*(fGlyphAtlas->fPixelMap.get()));
  tempPixMap.flipVertical();

  /* The get the pixel map from the foint atlas. */
  std::vector<uint8_t> atlasData = tempPixMap.toTGA();

  /* Flip the pixel map vertically since the origin is at the bottom left. */

  /* Conver the glyph atlas to a TGA and save it to file. */
  FILE * file = fopen("FontAtlas.tga", "wb");
  fwrite(atlasData.data(), sizeof(uint8_t), atlasData.size(), file);
  fclose(file);
}
