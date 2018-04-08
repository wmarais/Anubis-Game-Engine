#include "../../../Include/Anubis/Graphics/PixelMap.hpp"

using namespace Anubis::Common;
using namespace Anubis::Graphics;

const uint8_t PixelMap::kCompHeader[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

const uint8_t PixelMap::kUncompHeader[12] = {0,0,2,0,0,0,0,0,0,0,0,0};

const size_t PixelMap::kBPP[5] = {1, 3, 4, 3, 4};

/******************************************************************************/
PixelMap::PixelMap(const std::vector<uint8_t> data)
{
  /* Check if the TGA is uncompressed. */
  if(memcmp(kUncompHeader, data.data(), sizeof(kUncompHeader)) == 0)
  {
    /* Load the uncompressed image. */
    loadTGA(data, false);
  }
  else if(memcmp(kCompHeader, data.data(), sizeof(kCompHeader)) == 0)
  {
    /* Load a compressed tga. */
    loadTGA(data, true);
  }
  else
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Invalid TGA data provided.");
  }
}

/******************************************************************************/
PixelMap::PixelMap(size_t width, size_t height, PixelTypes type) :
  fType(type), fWidth(width), fHeight(height)
{
  /* Resize the data vector. */
  fData.resize(fWidth * fHeight * bpp());

  /* Clear the memory. */
  memset(fData.data(), 0, fData.size());
}

/******************************************************************************/
PixelMap::PixelMap(const uint8_t * data, size_t width, size_t height,
         PixelTypes type) : fType(type), fWidth(width), fHeight(height)
{
  /* Allocate the memory for the pixels. */
  fData.resize(fWidth * fHeight * bpp());

  /* Copy the pixels across. */
  memcpy(fData.data(), data, fData.size());
}

/******************************************************************************/
PixelMap & PixelMap::operator = (PixelMap && mv)
{
  /* Copy the properties. */
  fType = mv.fType;
  fWidth = mv.fWidth;
  fHeight = mv.fHeight;
  fBPP = mv.fBPP;

  /* Move the data across. */
  fData = std::move(mv.fData);

  /* Return the reference to this object. */
  return *this;
}

/******************************************************************************/
void PixelMap::loadTGA(const std::vector<uint8_t> & data, bool compressed)
{
  /* The current index to read from. */
  size_t curIndex = sizeof(kCompHeader);

  /* Read the image width. */
  fWidth = Memory::fromLittleEndian<uint16_t>(data, curIndex);
  curIndex += sizeof(uint16_t);

  /* Read the image height. */
  fHeight = Memory::fromLittleEndian<uint16_t>(data, curIndex);
  curIndex += sizeof(uint16_t);

  /* Get the image bpp. Note that it is bits per pixel, not bytes per pixel. */
  size_t bpp = data[curIndex];
  curIndex += 2;

  /* Check the information is valid. */
  if(fWidth == 0 || fHeight == 0 || ((bpp != 24) && (bpp != 32)))
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("TGA image has invalid properties, width: "
      << fWidth << " x " << fHeight << "px @ " << bpp << "bps.");
  }

  /* Calcualte the Bytes/pixel. */
  fBPP = bpp / 8;

  /* A TGA image is default formatted eitheras BGR or BGRA. */
  fType = fBPP == 3 ? PixelTypes::BGR : PixelTypes::BGRA;

  /* The amount of memory require in bytes. */
  size_t memLen = fWidth * fHeight * fBPP;

  /* Allocate memory for the image. */
  fData.resize(memLen);

  /* Make sure there is enough data to read the uncompressed pixels. */
  if(data.size() - curIndex < memLen)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Not enough data to read image pixels.");
  }

  /* Check if the TGA is not compressed. */
  if(!compressed)
  {
    /* Copy the pixel memory, no decompression required. */
    memcpy(fData.data(), data.data() + curIndex, memLen);
  }
  else
  {
    /* The index to read the data too. */
    size_t dstIndex = 0;

    /* The total number of pixels to read. */
    size_t pixelCount = fWidth * fHeight;

    /* The current number of pixels read. */
    size_t pixelsRead = 0;

    do
    {
      /* Read the current chunk header. */
      uint8_t chunkHeader = data[curIndex];
      curIndex++;

      /* Check if it's a RAW header. */
      if(chunkHeader < 128)
      {
        /* Add 1 to header to get the total number of raw pixels. */
        chunkHeader++;

        /* Calculate the read length. */
        chunkHeader *= fBPP;

        /* Read the chunk. */
        memcpy(fData.data() + dstIndex, data.data() + curIndex, chunkHeader);

        /* Increase the dst index. */
        dstIndex += chunkHeader;

        /* Increment the read index. */
        curIndex += chunkHeader;
      }
      /* Check if it's RLE encoded. */
      else
      {
        /* Subtract 127 to get rid of the ID bit. */
        chunkHeader -= 127;

        /* Copy the recurring pixel. */
        for(size_t i = 0; i < chunkHeader; i++)
        {
          memcpy(fData.data() + dstIndex, data.data() + curIndex, fBPP);
          dstIndex += fBPP;
        }

        /* Increment the read address by one pixel. */
        curIndex += fBPP;
      }

      /* Increment the pixels read. */
      pixelsRead += chunkHeader;
    }
    /* Keep looping until all the pixels are read. */
    while(pixelsRead < pixelCount);
  }
}

/******************************************************************************/
std::vector<uint8_t> PixelMap::toTGA() const
{
  /* The vector that will be returned. */
  std::vector<uint8_t> tga;

  /* Resize the vector to accept all the contents of the TGA image. Note that
   * +6 is for the Width, height and BPP (and one resrve byte). */
  tga.resize(sizeof(kUncompHeader) + kTGAHeaderLen + fData.size());

  /* Write the uncompressed TGA header. */
  memcpy(tga.data(), kUncompHeader, sizeof(kUncompHeader));

  /* Set the current write index. */
  size_t curIndex = sizeof(kUncompHeader);

  /* Write the width of the image. */
  writeLE<uint16_t>(tga.data() + curIndex, tga.size() - curIndex, fWidth);
  curIndex += sizeof(uint16_t);

  /* Write the height of the image. */
  writeLE<uint16_t>(tga.data() + curIndex, tga.size() - curIndex, fHeight);
  curIndex += sizeof(uint16_t);

  /* Write the BPP of the image. */
  *(tga.data() + curIndex) = bpp() * 8;
  curIndex += sizeof(uint8_t);

  /* Set the reserved byte to 0. */
  *(tga.data() + curIndex) = 0;
  curIndex += sizeof(uint8_t);

  /* Check the pixel type. */
  switch(fType)
  {
    /* A grayscale image, BGR and BGRA pixel maps can be directly written out
     * since they are in the correct order for TGA. */
    case PixelTypes::Gray:
    case PixelTypes::BGR:
    case PixelTypes::BGRA:
      /* Copy the memory. */
      memcpy(tga.data() + curIndex, fData.data(), fData.size());
    break;

    /* RGB and RGBA images need to swap the R and B channels swapped before
     * being written out. */
    case PixelTypes::RGB:
    case PixelTypes::RGBA:
      ANUBIS_THROW_RUNTIME_EXCEPTION("Functionality not implemented.");
    break;
  }

  /* Return the image data. */
  return tga;
}

///******************************************************************************/
//PixelMap & PixelMap::changePixelType(PixelTypes & pixelType)
//{
//  /* Check if it's the right type alrleady. */

//}

/******************************************************************************/
PixelMap::PixelTypes PixelMap::pixelType() const
{
  return fType;
}

/******************************************************************************/
size_t PixelMap::width() const
{
  return fWidth;
}

/******************************************************************************/
size_t PixelMap::height() const
{
  return fHeight;
}

/******************************************************************************/
size_t PixelMap::bpp() const
{
  return kBPP[static_cast<uint8_t>(fType)];
}

/******************************************************************************/
size_t PixelMap::stride() const
{
  return fWidth * bpp();
}
/******************************************************************************/
void PixelMap::resize(size_t width, size_t height)
{
  /* Store the new widht and height of the pixle map. */
  fWidth = width;
  fHeight = height;

  /* Resize the memory of the pixel map. */
  fData.resize(fWidth * fHeight * bpp());
}

/******************************************************************************/
void PixelMap::copyIn(const PixelMap & src, size_t dstX, size_t dstY)
{
  /* Check if the types match. */
  if(src.pixelType() != fType)
  {
    ANUBIS_THROW_RUNTIME_EXCEPTION("Pixel type mismatch.");
  }

  /* Check the coordinates are in range. */
  if(dstX >= fWidth || dstY >= fHeight)
  {
    /* Nothing to copy, just return. */
    return;
  }

  /* Calculate the maximum width to copy. */
  size_t cpWidth = src.width(); /*dstX + src.width() <= fWidth ? src.width() :
                    src.width() - (dstX + src.width() - fWidth);*/

  /* Calculate the maximum height to copy. */
  size_t cpHeight = src.height(); /*dstY + src.height() <= fHeight ? src.height() :
                    src.height() - (dstY + src.height() - fHeight);*/

  /* Iterate through all the rows and copy them line-by-line. */
  for(size_t r = 0; r < cpHeight; r++)
  {
    /* Calculate the source position to copy from. */
    size_t srcPos = r * src.width() * src.bpp();

    /* Calculate the destination to copy too. */
    size_t dstPos = (dstY + r) * stride() + (dstX * bpp());

    /* Copy the colums of each row. */
    memcpy(fData.data() + dstPos, src.fData.data() + srcPos, cpWidth * bpp());
  }

}


