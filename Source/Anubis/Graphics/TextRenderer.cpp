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

/*##############################################################################
##############################################################################*/
struct TextRenderer::Glyph final
{
  /** The code point of the glyph. */
  char32_t kCodePoint;

  /** The height of the font used to render this glyph. */
  const size_t fFontHeight;

  /** The area of the glyph. */
  const float kArea;

  /** The width of the glyph. */
  const float fWidth;

  /** The height of the glyph. */
  const float fHeight;

  /** The x bearing of the glyph. */
  const float fBearingX;

  /** They y bearing of the glyph. */
  const float fBearingY;

  /** How far to advance before rendering the next glyph. */
  const float fAdvanceX;

  const float fAdvanceY;

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
  size_t fontHeight) : kCodePoint(codePoint),
  fFontHeight(fontHeight), kArea(face->glyph->bitmap.width * face->glyph->bitmap.rows), fWidth(face->glyph->bitmap.width),
  fHeight(face->glyph->bitmap.rows), fBearingX(face->glyph->bitmap_left),
  fBearingY(face->glyph->bitmap_top), fAdvanceX(face->glyph->advance.x),
  fAdvanceY(face->glyph->advance.y)
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
  static const size_t kDefaultDim; // = 512;

  static const size_t kMaxDim = 8192;

  /** The minimum power of two to use when rezising the atlas pixel map. */
  static const size_t kMinPowerTwo = 9;

  /** The maximum power of two to use when resizing the atlas pixel map. */
  static const size_t kMaxPowerTwo = 13;

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

    ANUBIS_FORCE_INLINE bool isLeaf() const
    {
      return fLeft == nullptr && fRight == nullptr;
    }

    /***********************************************************************//**
     * Insert a new glyph into the atlas and pack it properly.
     *
     * @param glyph   The glyph to be inserted intgo the atlas.
     * @return        True if the glyph was packed or false if the glyph can not
     *                be packed because (1) this is a leaf node and it allready
     *                contains a glyph, (2) there is not enough space to fit
     *                the glyph.
     **************************************************************************/
    bool insert(std::shared_ptr<Glyph> glyph)
    {
      /* Check if this not a leaf node. If it's not then try to insert the glyph
       * into one of the child nodes. */
      if(!isLeaf())
      {
        /* Try to insert into the left node. */
        bool inserted = fLeft->insert(glyph);

        /* Check if it was not inserted on the left. */
        if(!inserted)
        {
          /* Try to insert to the right. */
          inserted = fRight->insert(glyph);
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
      if(!fRect.canFit(BoundingRect(0, 0, fGlyph->fWidth, fGlyph->fHeight)))
      {
        /* Return false to indicate that leaf node does not fit. */
        return false;
      }

      /* Check if the new glyph fits perfectly. */
      if(fRect.width() == glyph->fWidth && fRect.height() == glyph->fHeight)
      {
        /* Fit the glyph to the node. */
        fGlyph = glyph;

        /* Return true to indicate it was inserted. */
        return true;
      }

      /* Else the nodes must be split. */

      /* Create thje new children for the node. */
      fLeft = std::make_unique<Node>();
      fRight = std::make_unique<Node>();

      /* Decide which way to split the bounding rect. */
      size_t dw = fRect.width() - glyph->fWidth;
      size_t dh = fRect.height() - glyph->fHeight;

      /* Check if the bounding area must be split vertically. */
      if(dw > dh)
      {
        /* The left node refer to then left bounding rect. */
        fLeft->fRect = BoundingRect(fRect.x(), fRect.y(),
                                    glyph->fWidth, fRect.height());

        /* The right node refer to the right bounding rect. */
        fRight->fRect = BoundingRect(fRect.x() + glyph->fWidth, fRect.y(),
                              fRect.width() - glyph->fWidth, fRect.height());
      }
      /* else split the bounding rect horizontally. */
      else
      {
        /* The left node refers to the top bounding rect. */
        fLeft->fRect = BoundingRect(fRect.x(), fRect.y(),
                              fRect.width(), fRect.y() + glyph->fHeight);

        /* The right node refers to the bottom bounding rect. */
        fRight->fRect = BoundingRect(fRect.x(), fRect.y() + glyph->fHeight,
                                     fRect.width(), fRect.height());
      }

      /* Insert into the left node. */
      return fLeft->insert(glyph);
    }
  };

  /** The root node of the binary tree used to packe the glyphs. */
  std::unique_ptr<Node> fRoot;


  struct Compare
  {
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

  std::vector<std::shared_ptr<Glyph>> fGlyphsByArea;



  /** Indicate if the atlas changed or not. */
  bool fHasChanged;

  /** The pixel map of the atlas. */
  std::unique_ptr<PixelMap> fPixelMap;

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

  void resize(size_t reqDim);

  void pack(const BoundingRect &parentBR);

  void pack(std::shared_ptr<Glyph> glyph);



public:

  GlyphAtlas();
  ~GlyphAtlas();

  /*************************************************************************//**
   * Prepare the atlas by adding any non existing code points to the codepoint
   * mapping. This must be called before "pack()".
   *
   * @param str
   ****************************************************************************/
  void prepare(Font * font, const std::u32string &str, size_t fontHeight);

  /*************************************************************************//**
   * Tightly pack the glyphs into the glyph (texture) atlas.
   ****************************************************************************/
  void pack();
};

const size_t TextRenderer::GlyphAtlas::kDefaultDim = 512;

/******************************************************************************/
TextRenderer::GlyphAtlas::GlyphAtlas()
{
  /* Initialise the free type library. */
  if(FT_Init_FreeType(&fFTLibrary) != 0)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Failed to initialise FreeType.");
  }

  /* Create the default pixel map using the default atlas size. */
  fPixelMap = std::make_unique<PixelMap>(kDefaultDim, kDefaultDim,
                                         PixelMap::PixelTypes::Gray);
}

/******************************************************************************/
TextRenderer::GlyphAtlas::~GlyphAtlas()
{
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
      [](const char32_t & rhs, const std::shared_ptr<Glyph> & lhs)
        -> bool { return lhs->kCodePoint < rhs; });

    /* Search for the first glyph with the correct font height in the range,
     * there should never be more than one. */
    auto exact = std::find_if(start, end,
      [&fontHeight](const std::shared_ptr<Glyph> & glyph)
        -> bool { return glyph->fFontHeight == fontHeight; });

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
void TextRenderer::GlyphAtlas::resize(size_t reqDim)
{
  /* First check if it should be resized. */
  if(reqDim <= fPixelMap->width() && reqDim <= fPixelMap->height())
  {
    /* No need to resize. */
    return;
  }

  /* The dimension to resize too. */
  size_t resDim = 0;

  /* Calculate the next required power of two. Currently the maximum cache size
   * is 8192 x 8192. This is inline with what should work on most video cards.
   * It should be noted that this requires 64MB of RAM. */
  for(size_t n = kMinPowerTwo; n <= kMaxPowerTwo; n++)
  {
    /* Calculate the current power of two. */
    resDim = std::pow(2, n);

    /* Check if it's adequate. */
    if(reqDim <= resDim)
    {
      /* Create the new pixel map. */
      fPixelMap = std::make_unique<PixelMap>(resDim, resDim,
                                             PixelMap::PixelTypes::Gray);

      /* Return since there is nothing else left to do. */
      return;
    }
  }

  /* If this stage is reached, then the pixel map was not resized because the
   * required dimensions is larger than the maximum supported area. */
  ANUBIS_THROW_RUNTIME_EXCEPTION("Unable to resize Glyph Atlas, the required "
    "size is to large: " << reqDim << "x" << reqDim << ".");
}

/******************************************************************************/
void TextRenderer::GlyphAtlas::prepare(Font * font, const std::u32string & str,
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
      fGlyphs.insert(std::move(glyph));

      /* Set the glyph into the vector of glyphs sorted by height. */
      fGlyphsByArea.push_back(glyph);

      /* Mark the atlas as having changed. */
      fHasChanged = true;
    }
  }
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

    /* Iterate through all the glyphs. */
    for(; curIter != fGlyphsByArea.end(); curIter++)
    {
      /* Check if the glyph was sucessfully packed. */
      if(fRoot->insert(*curIter) == false)
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
struct TextRenderer::Data
{
  /** The number of vertices required to render a glyph. Since quads are used,
   * four vertices is required. */
  static const uint16_t kVertsPerGlyph = 4;

  /** The number of indices that is required to render a single glyph. Since two
   * triangles are used to render a single quad, six indices are required. */
  static const uint16_t kIndicesPerGlyph = 6;

  /** The maximum number of glyphs to cache. */
  const size_t kMaxGlyphCache;

  /** The maximum number of vertices to cache. */
  const size_t kMaxCachedVerts;

  /** The ID of the vertex buffer. */
  GLuint fVertBuffID;

  /** The ID of the index buffer. */
  GLuint fIndexBuffID;

  GLuint fVAOID;

  /** The vertex cache. */
  std::vector<Vector4f> fVertices;


  Data(const std::string &fontFile, const size_t fontHeight, size_t cacheLen);
  ~Data();


  void initCache();
};

/******************************************************************************/
TextRenderer::Data::Data(const std::string & fontFile, const size_t fontHeight,
                         size_t cacheLen) :
  kMaxGlyphCache(cacheLen),
  kMaxCachedVerts(cacheLen *  kVertsPerGlyph)
{


  /* Initialise the render cache. */
  initCache();
}

/******************************************************************************/
TextRenderer::Data::~Data()
{

}

/******************************************************************************/
void TextRenderer::Data::initCache()
{
  /* Calculate how many vertices are required. Since rectangles are required
   * that is 4 vertices per rectangle. */
  size_t reqVerts = kMaxGlyphCache * kVertsPerGlyph;

  /* Calculate how many indices must be created. Since the rectangles are built
   * from two triangles, that is 6 indexes per faces. */
  size_t reqIndices = kMaxGlyphCache * kIndicesPerGlyph;

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
    indices.push_back(1 + 2);

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
  fVertices.resize(kMaxCachedVerts);

  /* Create the vertex buffer. */
  glGenBuffers(1, &fVertBuffID);
  glBindBuffer(GL_ARRAY_BUFFER, fVertBuffID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4f) * reqVerts,
               fVertices.data(), GL_DYNAMIC_DRAW);
}

/******************************************************************************/
TextRenderer::TextRenderer(size_t cacheLen)
{
  /* Create the data object of the text renderer. */
  //fData = std::make_unique<Data>(cacheLen);


//  std::vector<GlyphInfo> glyphInfo;

//  /* Iterate through all the rows in the ascii image. */
//  for(size_t row = 0; row < 16; row++)
//  {
//    /* Iterate through all the columns in the ascii image. */
//    for(size_t col = 0; col < 16; col++)
//    {

//    }
//  }

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
