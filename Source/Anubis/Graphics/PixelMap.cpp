#include "../../../Include/Anubis/Graphics/PixelMap.hpp"

using namespace Anubis::Common;
using namespace Anubis::Graphics;

const uint8_t PixelMap::kCompHeader[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

const uint8_t PixelMap::kUncompHeader[12] = {0,0,2,0,0,0,0,0,0,0,0,0};

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
PixelMap & PixelMap::changePixelType(PixelTypes & pixelType)
{
  /* Check if it's the right type alrleady. */

}


